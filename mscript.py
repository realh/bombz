#!/usr/bin/python

import maitch
import mcommon
import mcode
import mgraphics

mgraphics.add_envs(mcommon.envs)
mcode.add_envs(mcommon.envs)
ctx = maitch.Context(**mcommon.envs)
mgraphics.init(ctx)
mcode.init(ctx)
ctx.run()
