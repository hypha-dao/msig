#pragma once

#include <eosio/binary_extension.hpp>
#include <eosio/eosio.hpp>
#include <eosio/ignore.hpp>
#include <eosio/transaction.hpp>

#include <document_graph/document_graph.hpp>
#include <document_graph/util.hpp>

using namespace eosio;

namespace hypha
{

   CONTRACT multisig : public eosio::contract
   {
   public:
      using eosio::contract::contract;

      DECLARE_DOCUMENT_GRAPH(multisig)

      // multisig(name self, name code, datastream<const char *> ds) : contract(self, code, ds) {}
      // ~multisig() {}

      ACTION erasedoc(const checksum256 &hash);
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
          * @param content_groups - Documentation to accompany a proposal
          */

      [[eosio::action]] void propose(eosio::name & proposer,
                                     eosio::name & proposal_name,
                                     ContentGroups & content_groups);

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
      [[eosio::action]] void cancel(name proposal_name, name canceler);
      [[eosio::action]] void complete(name proposal_name, name executer);

   private:
      void erase(name proposal_name, name eraser);
      DocumentGraph m_documentGraph = DocumentGraph(get_self());

      struct [[eosio::table, eosio::contract("multisig")]] proposal
      {
         name proposer;
         name proposal_name;
         checksum256 document_hash;

         uint64_t primary_key() const { return proposal_name.value; }

         EOSLIB_SERIALIZE(proposal, (proposer)(proposal_name)(document_hash))
      };

      typedef eosio::multi_index<name("proposals"), proposal> proposals;

   }; // namespace hypha
}
