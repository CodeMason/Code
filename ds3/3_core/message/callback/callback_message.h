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

#ifndef _EJA_CALLBACK_MESSAGE_H_
#define _EJA_CALLBACK_MESSAGE_H_

#include "message/data_message.h"

// fatal error C1189: #error:  WinSock.h has already been included
#include <msgpack.hpp>

namespace eja
{
	// Message
	class callback_message : public id_message
	{
	protected:
		callback_message(const callback_message& msg) : id_message(msg) { }
		callback_message(const message_type& type) : id_message(type) { }

		// Operator
		callback_message& operator=(const callback_message& msg);

	public:
		MSGPACK_DEFINE(MSGPACK_BASE(id_message));
	};

	// Operator
	inline callback_message& callback_message::operator=(const callback_message& msg)
	{
		if (this != &msg)
			id_message::operator=(msg);

		return *this;
	}

	// Factory
	template <message_type T>
	class callback_message_impl final : public callback_message
	{
		make_factory(callback_message_impl);

	public:
		callback_message_impl() : callback_message(T) { }
		callback_message_impl(const callback_message_impl& msg) : callback_message(msg) { }

		// Operator
		callback_message_impl& operator=(const callback_message_impl& msg);

	public:
		MSGPACK_DEFINE(MSGPACK_BASE(callback_message));
	};

	// Operator
	template <message_type T>
	callback_message_impl<T>& callback_message_impl<T>::operator=(const callback_message_impl& msg)
	{
		if (this != &msg)
			callback_message::operator=(msg);

		return *this;
	}
}

#endif
