//
//  reflection.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 2/2/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef reflection_hpp
#define reflection_hpp

#include <entt/meta/factory.hpp>

template <typename>
inline int refl_dummy_type;

#define REFL(TYPE, MEMBERS)                                                     \
  template <>                                                                   \
  inline int refl_dummy_type<TYPE> = []{                                        \
    using refl_t = TYPE;                                                        \
    entt::meta<TYPE>()                                                          \
      .type(#TYPE##_hs)                                                         \
        .prop("name"_hs, #TYPE)                                                 \
    MEMBERS;                                                                    \
    return 0;                                                                   \
  }()

#define REFL_EMPTY(TYPE)                                                        \
  template <>                                                                   \
  inline int refl_dummy_type<TYPE> = []{                                        \
    entt::meta<TYPE>()                                                          \
      .type(#TYPE##_hs)                                                         \
        .prop("name"_hs, #TYPE);                                                \
    return 0;                                                                   \
  }()

#define REFL_DATA(DATA)                                                         \
  .data<&refl_t::DATA>(#DATA##_hs)                                              \
    .prop("name"_hs, #DATA)

#endif
