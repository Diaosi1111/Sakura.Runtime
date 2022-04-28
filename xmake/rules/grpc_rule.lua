--!A cross-platform build utility based on Lua
--
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
--
--     http://www.apache.org/licenses/LICENSE-2.0
--
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.
--
-- Copyright (C) 2015-present, TBOOX Open Source Group.
--
-- @author      ruki
-- @file        xmake.lua
--

-- define rule: grpc.cpp
rule("grpc.cpp")
    set_extensions(".proto")
    on_load(function(target)
        import("grpc").load(target, "cxx")
    end)
    before_buildcmd_file(function (target, batchcmds, sourcefile_proto, opt)
        import("grpc").buildcmd(target, batchcmds, sourcefile_proto, opt, "cxx")
    end)
    before_build_files(function (target, batchjobs, sourcebatch, opt)
        import("grpc").build_batchjobs(target, batchjobs, sourcebatch, opt, "cxx")
    end, {batch = true})

