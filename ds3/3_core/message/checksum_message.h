//
// The MIT License(MIT)
//
// Copyright(c) 2014 Demonsaw LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef _EJA_CHECKSUM_MESSAGE_H_
#define _EJA_CHECKSUM_MESSAGE_H_

#include "system/type.h"

// fatal error C1189: #error:  WinSock.h has already been included
#include <msgpack.hpp>

namespace eja
{
	class checksum_message final
	{
		make_factory(checksum_message);

	protected:
		u32 m_checksum = static_cast<u32>(~0);

	public:
		checksum_message() { }
		checksum_message(const checksum_message& msg) : m_checksum(msg.m_checksum) { }

		// Operator
		checksum_message& operator=(const checksum_message& msg);

	public:
		// Has
		bool has_checksum() const { return m_checksum != 0; }

		// Set
		void set_checksum(const u32& checksum) { m_checksum = checksum; }

		// Get
		u32 get_checksum() const { return m_checksum; }
	};

	// Operator
	inline checksum_message& checksum_message::operator=(const checksum_message& msg)
	{
		if (this != &msg)
			m_checksum = msg.m_checksum;

		return *this;
	}
}

namespace msgpack
{
	MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS)
	{
		namespace adaptor
		{
			// Read
			template<>
			struct convert<eja::checksum_message>
			{
				msgpack::object const& operator()(msgpack::object const& o, eja::checksum_message& v) const
				{
					v.set_checksum(o.as<u32>());
					return o;
				}
			};

			// Write
			template<>
			struct pack<eja::checksum_message>
			{
				template <typename Stream>
				packer<Stream>& operator()(msgpack::packer<Stream>& o, eja::checksum_message const& v) const
				{
					o.pack_uint32(v.get_checksum());
					return o;
				}
			};
		}
	}
}

#endif
