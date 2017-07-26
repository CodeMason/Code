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

#include <QAction>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPoint>
#include <QTimer>
#include <QToolBar>

#include "component/callback/callback_action.h"
#include "component/callback/callback_type.h"
#include "component/message/message_acceptor_component.h"
#include "delegate/endpoint_delegate.h"
#include "entity/entity.h"
#include "entity/entity_table_view.h"
#include "font/font_awesome.h"
#include "model/router/router_client_model.h"
#include "widget/router/router_client_widget.h"

namespace eja
{
	// Constructor
	router_client_widget::router_client_widget(const entity::ptr entity, QWidget* parent /*= 0*/) : entity_widget(entity, "Clients", fa::user, callback_type::client, parent)
	{
		assert(thread_info::main());

		// Action
		m_update_action = make_action("Update", fa::bolt);

		// Menu
		add_button("Update", fa::bolt, [&]() { update(); });
		add_button("Popout", fa::clone, [&]() { popout(); });
		add_button("Close", fa::close, [&]() { close(); });

		// Layout
		auto hlayout = new QHBoxLayout;
		hlayout->setSpacing(0);
		hlayout->setMargin(0);

		hlayout->addWidget(m_icon);
		hlayout->addWidget(m_title);
		hlayout->addStretch(1);
		hlayout->addWidget(m_toolbar);

		m_titlebar = new QWidget(this);
		m_titlebar->installEventFilter(this);
		m_titlebar->setObjectName("menubar");
		m_titlebar->setLayout(hlayout);
		setTitleBarWidget(m_titlebar);

		// Table
		m_table = new entity_table_view(m_entity, this);
		m_model = new router_client_model(m_entity, { "Name" }, { 128 }, this);
		init(m_table, m_model);

		// Header
		const auto hheader = m_table->horizontalHeader();
		hheader->setSortIndicator(router_client_model::column::name, Qt::SortOrder::AscendingOrder);
		hheader->setStretchLastSection(true);
		hheader->setObjectName("stretch");

		// Callback
		add_callback(callback_action::add, [&](const entity::ptr entity) { m_model->add(entity); });
		add_callback(callback_action::remove, [&](const entity::ptr entity) { m_model->remove(entity); });
		add_callback(callback_action::clear, [&](const entity::ptr entity){ m_model->clear(); });

		// Delegate
		m_table->setItemDelegateForColumn(router_client_model::column::name, new endpoint_delegate(m_table));

		// Signal
		connect(m_table, &entity_table_view::customContextMenuRequested, this, &router_client_widget::show_menu);
		connect(m_update_action, &QAction::triggered, [&]() { update(); });

		// Sort
		m_table->sortByColumn(router_client_model::column::name);

		setWidget(m_table);
	}

	// Utility
	void router_client_widget::on_update()
	{
		assert(thread_info::main());

		// Service
		const auto acceptor = m_entity->get<message_acceptor_component>();
		if (acceptor->invalid())
			return;

		// Spam
		auto button = get_button(fa::bolt);
		if (button)
		{
			button->setEnabled(false);
			QTimer::singleShot(ui::refresh, [&]() { set_enabled(fa::bolt); });
		}

		m_model->update();
	}

	// Menu
	void router_client_widget::show_menu(const QPoint& pos)
	{
		assert(thread_info::main());

		const auto index = m_table->indexAt(pos);
		if (!index.isValid())
			return;

		QMenu menu(this);
		menu.addAction(m_update_action);

		menu.exec(m_table->viewport()->mapToGlobal(pos));
	}
}
