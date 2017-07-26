
import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item 
{
	// Variables
	property bool modified: state == "pulled" ? true : false;

	height: parent.height;
	width: parent.width;

	// Background
	Rectangle
	{
		anchors
		{
			fill: parent;
			top: parent.top;
			topMargin: padding_size_2;
		}
		color: light_grey;
		height: parent.height - padding_size;
		opacity: 0.25;
	}

	Row
	{
		spacing: padding_size_2;
		height: childrenRect.height;
		anchors 
		{
			top: parent.top;
			topMargin: font_size;
			horizontalCenter: parent.horizontalCenter;
		}

		Text
		{
			id: id_arrow;
			color: light_grey;			
			font
			{
				family: "FontAwesome";
				pixelSize: icon_size * 1.25;
			}
			text: fa_arrow_up;
			transformOrigin: Item.Center;
			width: icon_size_2;
			horizontalAlignment: Text.AlignHCenter;
			verticalAlignment: Text.AlignVCenter;

			Behavior on rotation 
			{ 
				NumberAnimation
				{ 
					duration: 200 
				}
			}
		}

		Text
		{
			id: id_text;
			anchors.verticalCenter: id_arrow.verticalCenter;
			color: light_grey;
			font
			{
				bold: true;
				pixelSize: title_size;
			}
			text: "Pull to clear       ";
		}
	}

	states: 
	[
		State 
		{
			name: "base"; when: -id_view.contentY >= (id_view.height - header_size_3);
			PropertyChanges
			{ 
				target: id_arrow;
				color: light_grey;
				rotation: 180;							
			}
		},
		State 
		{
			name: "pulled"; when: -id_view.contentY < (id_view.height - header_size_3);
			PropertyChanges
			{ 
				target: id_text; 
				text: "Release to clear" 
			}
			PropertyChanges
			{ 
				target: id_arrow; 
				color: blue;
				rotation: 0 
			}
		}
	]
}
