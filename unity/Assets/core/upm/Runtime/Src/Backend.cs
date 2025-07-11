/*
* Tencent is pleased to support the open source community by making Puerts available.
* Copyright (C) 2020 Tencent.  All rights reserved.
* Puerts is licensed under the BSD 3-Clause License, except for the third-party components listed in the file 'LICENSE' which may be subject to their corresponding license terms. 
* This file is subject to the terms and conditions defined in file 'LICENSE', which is part of this source code package.
*/


namespace Puerts
{
    public enum BackendType : int
    {
        V8 = 0,
        Node = 1,
        QuickJS = 2,
        Auto = 3
    }

    public abstract class Backend
    {
        protected JsEnv env;
        public Backend(JsEnv env)
        {
            this.env = env;
        }

        public abstract void LowMemoryNotification();
    }

    public class BackendV8 : Backend
    {
        public BackendV8(JsEnv env) : base(env)
        {
        }

        public bool IdleNotificationDeadline(double DeadlineInSeconds)
        {
#if THREAD_SAFE
            lock(this) {
#endif
            return PuertsDLL.IdleNotificationDeadline(env.isolate, DeadlineInSeconds);
#if THREAD_SAFE
            }
#endif
        }

        public override void LowMemoryNotification()
        {
#if THREAD_SAFE
            lock(this) {
#endif
            PuertsDLL.LowMemoryNotification(env.isolate);
#if THREAD_SAFE
            }
#endif
        }

        public void RequestMinorGarbageCollectionForTesting()
        {
#if THREAD_SAFE
            lock(this) {
#endif
            PuertsDLL.RequestMinorGarbageCollectionForTesting(env.isolate);
#if THREAD_SAFE
            }
#endif
        }

        public void RequestFullGarbageCollectionForTesting()
        {
#if THREAD_SAFE
            lock(this) {
#endif
            PuertsDLL.RequestFullGarbageCollectionForTesting(env.isolate);
#if THREAD_SAFE
            }
#endif
        }

        public void TerminateExecution()
        {
            PuertsDLL.TerminateExecution(env.isolate);
        }

    }

    public class BackendNodeJS : BackendV8
    {
        public BackendNodeJS(JsEnv env) : base(env)
        {
        }
    }

    public class BackendQuickJS : Backend
    {
        public BackendQuickJS(JsEnv env) : base(env)
        {
        }

        public override void LowMemoryNotification()
        {
#if THREAD_SAFE
            lock(this) {
#endif
#if PUERTS_DISABLE_IL2CPP_OPTIMIZATION || (!PUERTS_IL2CPP_OPTIMIZATION && UNITY_IPHONE) || !ENABLE_IL2CPP
            PuertsDLL.LowMemoryNotification(env.isolate);
#endif
#if THREAD_SAFE
            }
#endif
        }
    }
}