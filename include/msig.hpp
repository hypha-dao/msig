#pragma once

#include <eosio/binary_extension.hpp>
#include <eosio/eosio.hpp>
#include <eosio/ignore.hpp>
#include <eosio/transaction.hpp>

#include <document_graph.hpp>

using namespace eosio;

namespace hyphaspace
{

   class [[eosio::contract("msig")]] multisig : public contract
   {
   public:
      using contract::contract;

      multisig(name self, name code, datastream<const char *> ds) : contract(self, code, ds) {}
      ~multisig() {}

      ACTION erasedoc (const checksum256 &hash);
      /**
          * Create proposal
          *
          * @details Creates a proposal containing one transaction.
          * Allows an account `proposer` to make a proposal `proposal_name` which has `requested`
          * permission levels expected to approve the proposal, and if approved by all expected
          * permission levels then `trx` transaction can we executed by this proposal.
          * The `proposer` account is authorized and the `trx` transaction is verified if it was
          * authorized by the provided keys and permissions, and if the proposal name doesn’t
          * already exist; if all validations pass the `proposal_name` and `trx` trasanction are
          * saved in the proposals table and the `requested` permission levels to the
          * approvals table (for the `proposer` context). Storage changes are billed to `proposer`.
          *
          * @param proposer - The account proposing a transaction
          * @param proposal_name - The name of the proposal (should be unique for proposer)
          * @param requested - Permission levels expected to approve the proposal
          * @param content_groups - Documentation to accompany a proposal
          * @param trx - Proposed transaction
          */

      [[eosio::action]] void propose(eosio::ignore<name> &proposer,
                                       eosio::ignore<name> &proposal_name,
                                       eosio::ignore<std::set<permission_level>> &requested,
                                       eosio::ignore<std::vector<document_graph::content_group>> &content_groups,
                                       eosio::ignore<transaction> &trx);
      /**
          * Approve proposal
          *
          * @details Approves an existing proposal
          * Allows an account, the owner of `level` permission, to approve a proposal `proposal_name`
          * proposed by `proposer`. If the proposal's requested approval list contains the `level`
          * permission then the `level` permission is moved from internal `requested_approvals` list to
          * internal `provided_approvals` list of the proposal, thus persisting the approval for
          * the `proposal_name` proposal. Storage changes are billed to `proposer`.
          *
          * @param proposer - The account proposing a transaction
          * @param proposal_name - The name of the proposal (should be unique for proposer)
          * @param level - Permission level approving the transaction
          * @param proposal_hash - Transaction's checksum
          */
      [[eosio::action]] void approve(name proposer, name proposal_name, permission_level level,
                                     const eosio::binary_extension<eosio::checksum256> &proposal_hash);
      /**
          * Revoke proposal
          *
          * @details Revokes an existing proposal
          * This action is the reverse of the `approve` action: if all validations pass
          * the `level` permission is erased from internal `provided_approvals` and added to the internal
          * `requested_approvals` list, and thus un-approve or revoke the proposal.
          *
          * @param proposer - The account proposing a transaction
          * @param proposal_name - The name of the proposal (should be an existing proposal)
          * @param level - Permission level revoking approval for proposal
          */
      [[eosio::action]] void unapprove(name proposer, name proposal_name, permission_level level);
      /**
          * Cancel proposal
          *
          * @details Cancels an existing proposal
          *
          * @param proposer - The account proposing a transaction
          * @param proposal_name - The name of the proposal (should be an existing proposal)
          * @param canceler - The account cancelling the proposal (only the proposer can cancel an unexpired transaction, and the canceler has to be different than the proposer)
          *
          * Allows the `canceler` account to cancel the `proposal_name` proposal, created by a `proposer`,
          * only after time has expired on the proposed transaction. It removes corresponding entries from
          * internal proptable and from approval (or old approvals) tables as well.
          */
      [[eosio::action]] void cancel(name proposer, name proposal_name, name canceler);
      /**
          * Execute proposal
          *
          * @details Allows an `executer` account to execute a proposal.
          *
          * Preconditions:
          * - `executer` has authorization,
          * - `proposal_name` is found in the proposals table,
          * - all requested approvals are received,
          * - proposed transaction is not expired,
          * - and approval accounts are not found in invalidations table.
          *
          * If all preconditions are met the transaction is executed as a deferred transaction,
          * and the proposal is erased from the proposals table.
          *
          * @param proposer - The account proposing a transaction
          * @param proposal_name - The name of the proposal (should be an existing proposal)
          * @param executer - The account executing the transaction
          */
      [[eosio::action]] void exec(name proposer, name proposal_name, name executer);
      /**
          * Invalidate proposal
          *
          * @details Allows an `account` to invalidate itself, that is, its name is added to
          * the invalidations table and this table will be cross referenced when exec is performed.
          *
          * @param account - The account invalidating the transaction
          */
      [[eosio::action]] void invalidate(name account);

      // using propose_action = eosio::action_wrapper<name("propose"), &multisig::propose>;
      // using approve_action = eosio::action_wrapper<name("approve"), &multisig::approve>;
      // using unapprove_action = eosio::action_wrapper<name("unapprove"), &multisig::unapprove>;
      // using cancel_action = eosio::action_wrapper<name("cancel"), &multisig::cancel>;
      // using exec_action = eosio::action_wrapper<name("exec"), &multisig::exec>;
      // using invalidate_action = eosio::action_wrapper<name("invalidate"), &multisig::invalidate>;

   private:
      document_graph _document_graph = document_graph(get_self());

      struct approval
      {
         permission_level level;
         time_point time;
      };

      struct [[eosio::table]] proposal
      {
         name proposal_name;
         name proposer;
         std::vector<char> packed_transaction;
         std::vector<approval> requested_approvals;
         std::vector<approval> provided_approvals;
         checksum256 document_hash;

         uint64_t primary_key() const { return proposal_name.value; }

         EOSLIB_SERIALIZE(proposal, (proposal_name)(packed_transaction)(document_hash))
      };

      typedef eosio::multi_index<name("proposal"), proposal> proposals;

      struct [[eosio::table]] invalidation
      {
         name account;
         time_point last_invalidation_time;

         uint64_t primary_key() const { return account.value; }
      };

      typedef eosio::multi_index<name("invals"), invalidation> invalidations;

      struct [[eosio::table]] document
      {
         uint64_t id;
         checksum256 hash;
         name creator;
         vector<document_graph::content_group> content_groups;

         vector<document_graph::certificate> certificates;
         uint64_t primary_key() const { return id; }
         uint64_t by_creator() const { return creator.value; }
         checksum256 by_hash() const { return hash; }

         time_point created_date = current_time_point();
         uint64_t by_created() const { return created_date.sec_since_epoch(); }

         EOSLIB_SERIALIZE(document, (id)(hash)(creator)(content_groups)(certificates)(created_date))
      };

      typedef multi_index<name("documents"), document,
                          indexed_by<name("idhash"), const_mem_fun<document, checksum256, &document::by_hash>>,
                          indexed_by<name("bycreator"), const_mem_fun<document, uint64_t, &document::by_creator>>,
                          indexed_by<name("bycreated"), const_mem_fun<document, uint64_t, &document::by_created>>>
          document_table;
   };
   /** @}*/ // end of @defgroup multisig msig.hypha
} // namespace hyphaspace
