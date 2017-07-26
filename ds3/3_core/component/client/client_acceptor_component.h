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

#ifndef _EJA_CLIENT_ACCEPTOR_COMPONENT_H_
#define _EJA_CLIENT_ACCEPTOR_COMPONENT_H_

#include <memory>

#include "component/service/acceptor_service_component.h"
#include "http/http_code.h"
#include "message/message_packer.h"
#include "system/type.h"

// fatal error C1189: #error:  WinSock.h has already been included
#include <msgpack.hpp>

namespace eja
{
	class entity;
	class http_request;
	class http_response;
	class http_socket;
	class message_packer;

	class client_acceptor_component final : public acceptor_service_component
	{
		make_factory(client_acceptor_component);

	private:
		enum class async_state { none, close, read };

	private:
		// Interface
		ptr shared_from_this() { return shared_from_base<client_acceptor_component>(); }
		
		// Utility
		virtual void close() override;
		void close(const std::shared_ptr<entity> entity);		

		// Commands
		void async_open();
		void open();
		
		// Request
		void read_request(const std::shared_ptr<http_socket> socket);
		void read_request(const std::shared_ptr<http_socket> socket, const std::shared_ptr<http_request> request);

		// Response
		void write_response(const std::shared_ptr<http_socket> socket);
		void write_response(const std::shared_ptr<http_socket> socket, const http_code code, const async_state state);
		void write_response(const std::shared_ptr<http_socket> socket, const std::shared_ptr<http_response> response, const async_state state);

		void write_ok(const std::shared_ptr<http_socket> socket) { write_response(socket, http_code::ok, async_state::close); }
		void write_not_found(const std::shared_ptr<http_socket> socket) { write_response(socket, http_code::not_found, async_state::close); }
		void write_internal_service_error(const std::shared_ptr<http_socket> socket) { write_response(socket, http_code::internal_service_error, async_state::close); }
		void write_not_implemented(const std::shared_ptr<http_socket> socket) { write_response(socket, http_code::not_implemented, async_state::close); }
		void write_unauthorized(const std::shared_ptr<http_socket> socket) { write_response(socket, http_code::unauthorized, async_state::close); }

		// Has
		bool has_redirect(const std::shared_ptr<http_request> request);

		// Set
		using service_component::set_status;
		virtual void set_status() override;

		// Get
		using acceptor_service_component::get_entity;
		std::shared_ptr<entity> get_entity(const std::shared_ptr<http_request> request) const;

	public:
		client_acceptor_component() { }
		client_acceptor_component(const client_acceptor_component& comp) : acceptor_service_component(comp) { }
		client_acceptor_component(const size_t threads) : acceptor_service_component(threads) { }

		// Operator
		client_acceptor_component& operator=(const client_acceptor_component& comp);

		// Interface
		virtual void clear() override;

		// Utility
		virtual void start() override;
		virtual void restart() override;
		virtual void stop() override;
	};
}

#endif
