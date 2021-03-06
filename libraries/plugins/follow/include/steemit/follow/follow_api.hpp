#pragma once

#include <steemit/app/application.hpp>
#include <steemit/chain/comment_object.hpp>

#include <steemit/follow/follow_objects.hpp>

#include <fc/api.hpp>

namespace steemit { namespace follow {

using std::vector;
using std::string;

struct feed_entry
{
   string   author;
   string   permlink;
   string   reblog_by;
   uint32_t entry_id = 0;
};

struct comment_feed_entry
{
   comment_object comment;
   string         reblog_by;
   uint32_t       entry_id = 0;
};

struct account_reputation
{
   string      account;
   share_type  reputation;
};

namespace detail
{
   class follow_api_impl;
}

class follow_api
{
   public:
      follow_api( const app::api_context& ctx );

      void on_api_startup();

      vector< follow_object > get_followers( string to, string start, follow_type type, uint16_t limit )const;
      vector< follow_object > get_following( string from, string start, follow_type type, uint16_t limit )const;

      vector< feed_entry > get_feed_entries( string account, uint32_t entry_id = 0, uint16_t limit = 500 )const;
      vector< comment_feed_entry > get_feed( string account, uint32_t entry_id = 0, uint16_t limit = 500 )const;
      vector< account_reputation > get_account_reputations( string lower_bound_name, uint32_t limit = 1000 )const;

   private:
      std::shared_ptr< detail::follow_api_impl > my;
};

} } // steemit::follow

FC_REFLECT( steemit::follow::feed_entry, (author)(permlink)(reblog_by)(entry_id) );
FC_REFLECT( steemit::follow::comment_feed_entry, (comment)(reblog_by)(entry_id) );
FC_REFLECT( steemit::follow::account_reputation, (account)(reputation) );

FC_API( steemit::follow::follow_api, (get_followers)(get_following)(get_feed_entries)(get_feed)(get_account_reputations) )
