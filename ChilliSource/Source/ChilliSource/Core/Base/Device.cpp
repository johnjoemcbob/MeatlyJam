//
//  Device.cpp
//  Chilli Source
//  Created by Scott Downie on 03/05/2011.
//
//  The MIT License (MIT)
//
//  Copyright (c) 2011 Tag Games Limited
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include <ChilliSource/Core/Base/Device.h>

#ifdef CS_TARGETPLATFORM_ANDROID
#include <CSBackend/Platform/Android/Core/Base/Device.h>
#endif

#ifdef CS_TARGETPLATFORM_IOS
#import <CSBackend/Platform/iOS/Core/Base/Device.h>
#endif

#ifdef CS_TARGETPLATFORM_WINDOWS
#include <CSBackend/Platform/Windows/Core/Base/Device.h>
#endif

namespace ChilliSource
{
    namespace Core
    {
        CS_DEFINE_NAMEDTYPE(Device);
        //--------------------------------------------------
        //--------------------------------------------------
        DeviceUPtr Device::Create()
        {
#if defined CS_TARGETPLATFORM_ANDROID
            return DeviceUPtr(new CSBackend::Android::Device());
#elif defined CS_TARGETPLATFORM_IOS
            return DeviceUPtr(new CSBackend::iOS::Device());
#elif defined CS_TARGETPLATFORM_WINDOWS
            return DeviceUPtr(new CSBackend::Windows::Device());
#else
            return nullptr;
#endif
        }
    }
}
