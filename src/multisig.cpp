#include <eosio/action.hpp>
#include <eosio/crypto.hpp>
#include <eosio/permission.hpp>
#include <eosio/ignore.hpp>
#include <multisig.hpp>

#include <document_graph/document_graph.hpp>
#include <document_graph/util.hpp>
#include <document_graph/document.hpp>

namespace hypha
{

   void multisig::erasedoc(const checksum256 &hash)
   {
      m_documentGraph.eraseDocument(hash);
   }

   void multisig::propose(eosio::name &proposer,
                          eosio::name &proposal_name,
                          ContentGroups &content_groups)
   {
      require_auth(proposer);

      Document doc(get_self(), proposer, content_groups);

      proposals proptable(get_self(), get_self().value);
      check(proptable.find(proposal_name.value) == proptable.end(), "proposal with the same name exists");

      proptable.emplace(get_self(), [&](auto &prop)
                        {
                           prop.proposer = proposer;
                           prop.proposal_name = proposal_name;
                           prop.document_hash = doc.getHash();
                        });
   }

   void multisig::erase(name proposal_name, name eraser)
   {
      // require_auth(eraser);

      proposals proptable(get_self(), get_self().value);
      auto &prop = proptable.get(proposal_name.value, "proposal not found");

      eosio::check(prop.proposer == eraser || eraser == get_self(), "canceler must be proposer or get_self");

      m_documentGraph.eraseDocument(prop.document_hash);
      proptable.erase(prop);
   }

   void multisig::cancel(name proposal_name, name canceler)
   {
      require_auth(canceler);
      erase(proposal_name, canceler);
   }

   void multisig::complete(name proposal_name, name executer)
   {
      require_auth(executer);
      erase(proposal_name, executer);
   }

} // namespace hypha
