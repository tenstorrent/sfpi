/*
 * SPDX-FileCopyrightText: © 2023 Tenstorrent Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#if 0
// Goal is for gimple to look like this

with (int x)
{
  int D.1726;

  {
    struct thing one;

    try
      {
        sfpi::thing::thing (&one);
        try
          {
            if (x <= 14) goto <D.1723>; else goto <D.1724>;
            <D.1723>:
            x = x + 10;
            goto <D.1725>;
            <D.1724>:
            <D.1725>:
          }
        finally
          {
            sfpi::thing::~thing (&one);
          }
      }
    finally
      {
        one = {CLOBBER};
      }
  }
  x = x + 1;
  D.1726 = x;
  return D.1726;
}
#endif

namespace sfpi {

    class thing {
        int x;
    public:
        thing() : x(0) {}
        ~thing() { }
    };
}
    
int with(int x)
{
    {
        sfpi::thing one;
        if (x + 5 < 20) {
            x = x + 10;
        }
    }

    x = x + 1;
    
    return x;
}
    
int without(int x)
{
    if (x + 5 < 20) {
        x = x + 10;
    }
    
    x = x + 1;

    return x;
}
