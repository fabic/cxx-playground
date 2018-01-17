#ifndef _PIMPL_PLUGIN_types_HPP
#define _PIMPL_PLUGIN_types_HPP

namespace clong {

  /// The type that matches our database primary keys. Specifically column `id`
  /// of table `decl`.
  /// Note that PostgreSQL does not support unsigned integer types:
  /// `using DBIdentifier_t = unsigned int ;`
  /// So we _have to_ use `int`.
  using DBIdentifier_t = int ;

} // clong ns


#endif // _PIMPL_PLUGIN_types_HPP
