#pragma once

#include <steemit/follow/follow_plugin.hpp>

#include <steemit/chain/protocol/types.hpp>

#include <graphene/db/generic_index.hpp>
#include <boost/multi_index/composite_key.hpp>

namespace steemit { namespace follow {

using namespace std;
using namespace steemit::chain;
using graphene::db::abstract_object;

#ifndef FOLLOW_SPACE_ID
#define FOLLOW_SPACE_ID 8
#endif

enum follow_object_type
{
   follow_object_type,
   feed_object_type,
   reputation_object_type
};

enum follow_type
{
   undefined,
   blog,
   ignore
};

class follow_object : public abstract_object< follow_object >
{
   public:
      static const uint8_t space_id = FOLLOW_SPACE_ID;
      static const uint8_t type_id  = follow_object_type;

      string               follower;
      string               following;
      set< follow_type >   what; /// blog
};

class feed_object : public abstract_object< feed_object >
{
   public:
      static const uint8_t space_id = FOLLOW_SPACE_ID;
      static const uint8_t type_id  = feed_object_type;

      account_id_type account;
      account_id_type reblogged_by;
      comment_id_type comment;
      uint32_t        account_feed_id = 0;
};

class reputation_object : public abstract_object< reputation_object >
{
   public:
      static const uint8_t space_id = FOLLOW_SPACE_ID;
      static const uint8_t type_id  = reputation_object_type;

      account_id_type account;
      share_type      reputation;
};

struct by_following_follower;
struct by_follower_following;

using namespace boost::multi_index;

typedef multi_index_container<
   follow_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< object, object_id_type, &object::id > >,
      ordered_unique< tag< by_following_follower >,
         composite_key< follow_object,
            member< follow_object, string, &follow_object::following >,
            member< follow_object, string, &follow_object::follower >
         >,
         composite_key_compare< std::less<string>, std::less<string> >
      >,
      ordered_unique< tag< by_follower_following >,
         composite_key< follow_object,
            member< follow_object, string, &follow_object::follower >,
            member< follow_object, string, &follow_object::following >
         >,
         composite_key_compare< std::less<string>, std::less<string> >
      >
   >
> follow_multi_index_type;

struct by_feed;
struct by_old_feed;
struct by_account;
struct by_comment;

typedef multi_index_container<
   feed_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< object, object_id_type, &object::id > >,
      ordered_unique< tag< by_feed >,
         composite_key< feed_object,
            member< feed_object, account_id_type, &feed_object::account >,
            member< feed_object, uint32_t, &feed_object::account_feed_id >
         >,
         composite_key_compare< std::less< account_id_type >, std::greater< uint32_t > >
      >,
      ordered_unique< tag< by_old_feed >,
         composite_key< feed_object,
            member< feed_object, account_id_type, &feed_object::account >,
            member< feed_object, uint32_t, &feed_object::account_feed_id >
         >,
         composite_key_compare< std::less< account_id_type >, std::less< uint32_t > >
      >,
      ordered_unique< tag< by_account >,
         composite_key< feed_object,
            member< feed_object, account_id_type, &feed_object::account >,
            member< object, object_id_type, &object::id >
         >
      >,
      ordered_unique< tag< by_comment >,
         composite_key< feed_object,
            member< feed_object, comment_id_type, &feed_object::comment >,
            member< feed_object, account_id_type, &feed_object::account >
         >
      >
   >
> feed_multi_index_type;

struct by_reputation;

typedef multi_index_container<
   reputation_object,
   indexed_by<
      ordered_unique< tag< by_id >, member< object, object_id_type, &object::id > >,
      ordered_unique< tag< by_reputation >,
         composite_key< reputation_object,
            member< reputation_object, share_type, &reputation_object::reputation >,
            member< reputation_object, account_id_type, &reputation_object::account >
         >,
         composite_key_compare< std::greater< share_type >, std::less< account_id_type > >
      >,
      ordered_unique< tag< by_account >, member< reputation_object, account_id_type, &reputation_object::account > >
   >
> reputation_multi_index_type;

typedef graphene::db::generic_index< follow_object,      follow_multi_index_type >     follow_index;
typedef graphene::db::generic_index< feed_object,        feed_multi_index_type >       feed_index;
typedef graphene::db::generic_index< reputation_object,  reputation_multi_index_type > reputation_index;

} } // steemit::follow

FC_REFLECT_ENUM( steemit::follow::follow_type, (undefined)(blog)(ignore) )

FC_REFLECT_DERIVED( steemit::follow::follow_object, (graphene::db::object), (follower)(following)(what) )
FC_REFLECT_DERIVED( steemit::follow::feed_object, (graphene::db::object), (account)(reblogged_by)(comment)(account_feed_id) )
FC_REFLECT_DERIVED( steemit::follow::reputation_object, (graphene::db::object), (account)(reputation) )
