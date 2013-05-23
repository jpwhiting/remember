import QtQuick 1.1
import com.nokia.meego 1.0
import com.jpwhiting 1.0

Page {
    tools: commonTools
    property string title;

    Component {
        id: tasksDelegate;
        Item {
            width: parent.width;
            height: nameText.height + subText.height + 30;

            Rectangle {
                id: priorityRect;
                anchors.top: parent.top;
                anchors.left: parent.left;
                anchors.right: checkBox.left;
                anchors.margins: 5;
                anchors.bottom: parent.bottom;
                color: (priority == 1 ? "darkorange" :
                        (priority == 2 ? "darkblue" :
                         (priority == 3 ? "darkcyan" : "darkgrey")));
                width: 15;
            }

            CheckBox {
                id: checkBox;
                anchors.verticalCenter: nameText.verticalCenter;
                anchors.left: parent.left;
                anchors.leftMargin: 20;
                onClicked: remember.selectTask(index, checkBox.checked);
            }

            Label {
                id: nameText;
                anchors.left: checkBox.right;
                anchors.leftMargin: 10;
                anchors.right: arrowIcon.left;
                anchors.rightMargin: 20;
                anchors.top: parent.top;
                anchors.topMargin: 10;
                text: display;
            }

            Image {
                id: arrowIcon;
                source: "image://theme/icon-m-common-drilldown-arrow";
                anchors.right: parent.right;
                anchors.verticalCenter: parent.verticalCenter;
                anchors.rightMargin: 20;
            }

            MouseArea {
                id: tagMouse
                anchors.left: nameText.left;
                anchors.right: arrowIcon.right;
                anchors.top: parent.top;
                anchors.bottom: parent.bottom;
                onClicked: {
                    remember.setCurrentTask(index);
                    appWindow.editTask();
                }
            }

            Item {
                id: subText
                anchors.top: nameText.bottom;
                anchors.topMargin: 10;
                anchors.left: parent.left;
                anchors.leftMargin: 20;
                anchors.right: arrowIcon.left;
                anchors.rightMargin: 20;
                height: childrenRect.height;

                Row {
                    id: tagsText;
                    anchors.top: parent.top;
                    anchors.left: parent.left;
                    height: childrenRect.height;
                    spacing: 5;

                    Repeater {
                        model: tags;

                        Rectangle {
                            color: "lightsteelblue";
                            width: childrenRect.width;
                            height: childrenRect.height;

                            Text {
                                text: modelData;
                                font.pixelSize: 15;
                            }
                        }
                    }

                }

                Text {
                    id: duedateText;
                    anchors.top: parent.top;
                    anchors.right: parent.right;
                    text: Qt.formatDate(duedate, session.dateFormat());
                    font.pixelSize: 15;
                }
            }

            Rectangle {
                id: line
                y: parent.height
                width: parent.width
                height: 1
                color: "#bbb"
                opacity: 0.8
            }
        }
    }

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

            text: title;
            font.pixelSize: 32;
            color: "white";
        }
    }

    ListView {
        id: tasksView;
        width: parent.width;
        height: parent.height - label.height;
        anchors {
            top: label.bottom;
            left: parent.left;
        }

        model: remember.tasksModel;
        delegate: tasksDelegate;
        focus: true;
        clip: true;
    }
}
