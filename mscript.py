#!/usr/bin/python

import maitch
import mcommon
import mgraphics

mgraphics.add_envs(mcommon.envs)
ctx = maitch.Context(**mcommon.envs)
mgraphics.init(ctx)
ctx.run()
