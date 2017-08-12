#include "rov_extensions.hpp"
#include <nil/data/bounding_box.hpp>
#include <rov/rov.hpp>


namespace Nil_ext {


void
rov_render_bounding_box(
  Nil::Data::Bounding_box bbs[],
  size_t count)
{
  for(size_t i = 0; i < count; ++i)
  {
    const float *min = bbs[i].min;
    const float *max = bbs[i].max;

    // Y lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { min[0], max[1], min[2] };
      rov_submitLine(a, b);

      const float c[3] { max[0], min[1], min[2] };
      const float d[3] { max[0], max[1], min[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], min[1], max[2] };
      const float f[3] { min[0], max[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { max[0], min[1], max[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }

    // X Lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { max[0], min[1], min[2] };
      rov_submitLine(a, b);

      const float c[3] { min[0], max[1], min[2] };
      const float d[3] { max[0], max[1], min[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], min[1], max[2] };
      const float f[3] { max[0], min[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { min[0], max[1], max[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }

    // Z lines
    {
      const float a[3] { min[0], min[1], min[2] };
      const float b[3] { min[0], min[1], max[2] };
      rov_submitLine(a, b);

      const float c[3] { max[0], min[1], min[2] };
      const float d[3] { max[0], min[1], max[2] };
      rov_submitLine(c, d);

      const float e[3] { min[0], max[1], min[2] };
      const float f[3] { min[0], max[1], max[2] };
      rov_submitLine(e, f);

      const float g[3] { max[0], max[1], min[2] };
      const float h[3] { max[0], max[1], max[2] };
      rov_submitLine(g, h);
    }
  }
}


} // ns
