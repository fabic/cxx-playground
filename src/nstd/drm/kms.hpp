#ifndef _DUDE_DRM_DRM_H
#define _DUDE_DRM_DRM_H

namespace linux {
namespace drm {

  class kms {
    public:
      int open(const char *node);
  };

} // drm ns.
} // linux ns.

#endif // _DUDE_DRM_DRM_H

