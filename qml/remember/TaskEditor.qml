import QtQuick 1.1
import com.nokia.meego 1.0
import com.jpwhiting 1.0

Page {
    tools: commonTools;

    Rectangle {
        id: label;
        width: parent.width;
        height: headerText.height + 24;
        color: "mediumblue";
        anchors {
            top: parent.top;
            left: parent.left;
        }

        Text {
            id: headerText;
            anchors {
                left: parent.left;
                leftMargin: 20;
                right: parent.right;
                verticalCenter: parent.verticalCenter;
            }

            text: "Edit task";
            font.pixelSize: 32;
            color: "white";
        }
    }

    Flickable {
        id: container
        anchors.top: label.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        contentWidth: col.width
        contentHeight: col.height
        flickableDirection: Flickable.VerticalFlick
        pressDelay: 100
        clip: true

        Column {
            id: col
            spacing: 5
            width: container.width

            Label {
                text: "Task Name:"
            }

            TextArea {
                id: edit
                anchors.left: parent.left
                anchors.right: parent.right
                height: 100
                text: appWindow.chosenTask.name;
                wrapMode: TextEdit.Wrap
            }

            Label {
                text: "Due:"
            }

            Text {
                id: duedateText;
                height: 30;
                text: Qt.formatDate(appWindow.chosenTask.duedate);
                font.pixelSize: 25;
            }

            Label {
                text: "Tags:"
            }

            Row {
                id: tagsrow;
                spacing: 5;

                Repeater {
                    model: appWindow.chosenTask.tags;

                    Rectangle {
                        color: "lightsteelblue";
                        width: childrenRect.width;
                        height: childrenRect.height;

                        Text {
                            text: modelData;
                            font.pixelSize: 25;
                        }
                    }
                }
            }
        }
    }
}
