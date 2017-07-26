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

#include <cassert>

#include <QColor>

#include "component/endpoint_component.h"
#include "component/group_ui_component.h"
#include "controller/application_controller.h"
#include "entity/entity.h"
#include "model/client_model.h"
#include "thread/thread_info.h"

namespace eja
{
	// Interface
	void client_model::on_add(const entity::ptr entity)
	{
		assert(thread_info::main());

		const auto row = m_list.size();

		beginInsertRows(QModelIndex(), row, row);
		m_list.push_back(entity);
		endInsertRows();

		//sort();
	}

	void client_model::on_clear()
	{
		assert(thread_info::main());

		beginResetModel();
		m_list.clear();
		endResetModel();
	}

	// Utility
	bool client_model::empty() const
	{
		assert(thread_info::main());

		return m_list.empty();
	}

	void client_model::sort()
	{
		beginResetModel();

		m_list->sort([](const entity::ptr e1, const entity::ptr e2)
		{
			const auto ec1 = e1->get<endpoint_component>();
			const auto ec2 = e2->get<endpoint_component>();
			return boost::algorithm::to_lower_copy(ec1->get_name()) < boost::algorithm::to_lower_copy(ec2->get_name());
		});

		endResetModel();
	}

	// Model
	bool client_model::removeRows(int row, int count, const QModelIndex& parent /*= QModelIndex()*/)
	{
		assert(thread_info::main() && (count == 1));

		const auto size = m_list.size();
		auto it = m_list.cbegin();
		std::advance(it, row);

		beginRemoveRows(QModelIndex(), row, row);
		m_list.erase(it);
		endRemoveRows();

		// Callback
		return m_list.size() < size;
	}

	QVariant client_model::data(const QModelIndex& index, int role) const
	{
		assert(thread_info::main());

		if (!index.isValid())
			return QVariant();

		const size_t row = static_cast<size_t>(index.row());
		const auto entity = get_entity(row);
		if (!entity)
			return QVariant();

		size_t col;
		if (role <= Qt::UserRole)
		{
			col = static_cast<size_t>(index.column());
		}
		else
		{
			col = role - (Qt::UserRole + 1);
			role = Qt::DisplayRole;
		}

		switch (role)
		{
			case Qt::DisplayRole:
			{
				switch (col)
				{
					case column::name:
					{
						const auto endpoint = entity->get<endpoint_component>();
						return QString::fromStdString(endpoint->get_name());
					}
					case column::first:
					{
						const auto group_ui = entity->get<group_ui_component>();
						return group_ui->get_initial();
					}
					case column::color:
					{
						QColor color;
						const auto endpoint = entity->get<endpoint_component>();

						if (endpoint->get_alpha() >= 8)
							color.setRgba(endpoint->get_color());
						else
							color.setRgb(endpoint->get_red(), endpoint->get_green(), endpoint->get_blue()); 

						return color;

					}
					case column::color_inverse:
					{						
						const auto endpoint = entity->get<endpoint_component>();

						if (endpoint->get_alpha() >= 8)
						{
							QColor color(255 - endpoint->get_red(), 255 - endpoint->get_green(), 255 - endpoint->get_blue(), endpoint->get_alpha());
							return color;
						}							
						else
						{
							QColor color(255 - endpoint->get_red(), 255 - endpoint->get_green(), 255 - endpoint->get_blue());
							return color;
						}												
					}					
					case column::contrast:
					{
						const auto endpoint = entity->get<endpoint_component>();
						const double value = ((0.299 * endpoint->get_red()) + (0.587 * endpoint->get_green()) + 0.114 * endpoint->get_blue()) / 255.0;
						return QVariant::fromValue(value);
					}
					case column::selected:
					{
						const auto app = application_controller::get();
						const auto selected =  entity == app->get_entity();
						return selected;
					}
				}

				break;
			}
		}

		return QVariant();
	}

	int client_model::rowCount(const QModelIndex& parent /*= QModelIndex()*/) const
	{
		assert(thread_info::main());

		return m_list.size();
	}

	QHash<int, QByteArray> client_model::roleNames() const
	{
		static QHash<int, QByteArray> roles;
		roles[column::role_name] = "role_name";
		roles[column::role_first] = "role_first";
		roles[column::role_color] = "role_color";
		roles[column::role_color_inverse] = "role_color_inverse";
		roles[column::role_contrast] = "role_contrast";
		roles[column::role_selected] = "role_selected";

		return roles;
	}

	// Get
	entity::ptr client_model::get_entity(const size_t row) const
	{
		assert(thread_info::main());

		if (row < m_list.size())
		{
			auto it = m_list.begin();
			std::advance(it, row);
			return *it;
		}

		return nullptr;
	}

	size_t client_model::get_row(const entity::ptr entity) const
	{
		assert(thread_info::main());

		size_t pos = 0;
		for (const auto& e : m_list)
		{
			if (e == entity)
				return pos;

			++pos;
		}

		return npos;
	}
}
