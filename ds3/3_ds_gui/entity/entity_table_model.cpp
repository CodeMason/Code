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

#include "entity/entity.h"
#include "entity/entity_table_model.h"
#include "thread/thread_info.h"

namespace eja
{
	// Utility
	QVariant entity_table_model::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
	{
		if (section >= 0)
		{
			switch (orientation)
			{
				case Qt::Horizontal:
				{
					switch (role)
					{
						case Qt::DisplayRole:
						case Qt::UserRole:
						{
							const size_t col = static_cast<size_t>(section);
							return QString::fromStdString(m_column_names[col]);
						}
						case Qt::TextAlignmentRole:
						{
							return static_cast<int>(Qt::AlignVCenter | Qt::AlignLeft);
						}
					}

					break;
				}
				case Qt::Vertical:
				{
					switch (role)
					{
						case Qt::TextAlignmentRole:
						{
							return static_cast<int>(Qt::AlignVCenter | Qt::AlignRight);
						}
					}

					break;
				}
			}
		}

		return QAbstractItemModel::headerData(section, orientation, role);
	}

	// Get
	entity::ptr entity_table_model::get_entity(const QModelIndex& index) const
	{
		assert(thread_info::main());

		if (!index.isValid())
			return nullptr;

		return get_entity(index.row());
	}
}
