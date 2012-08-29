import QtQuick 1.1
import com.nokia.meego 1.0
import com.jpwhiting 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage;

    function showTasks(name, id)
    {
        tasksPage.title = name;
        remember.setListId(id);
        pageStack.push(tasksPage);
    }

    function editTask()
    {
        pageStack.push(taskEditor);
    }

    Connections {
        target: session;
        onTokenCheck: {
            if (!success)
            {
                loginDialog.open();
                loginDialog.url = session.getAuthUrl();
            }
        }
    }

    LoginDialog {
        id: loginDialog;
    }

    MainPage {
        id: mainPage;
    }

    TasksPage {
        id: tasksPage;
    }

    TaskEditor {
        id: taskEditor;
    }

    Dialog {
        id: newTaskDialog;
        title: Label {
            text: (pageStack.currentPage == tasksPage ? "New task" : "New list")
            platformStyle: LabelStyle {
                textColor: "white"
            }
        }
        content: Item {
                width: parent.width + 20;
                height: 120;

                TextArea {
                    id: edit
                    width: parent.width
                    height: 100
                    placeholderText: (pageStack.currentPage == tasksPage ?
                                          "Enter task name" : "Enter list name")
                    wrapMode: TextEdit.Wrap
                }
            }
        buttons: ButtonRow {
            id: buttonRow;
            anchors.horizontalCenter: parent.horizontalCenter
            Button {
                text: "Cancel"
                onClicked: {
                    newTaskDialog.reject();
                }
            }

            Button {
                text: "Add"
                onClicked: {
                    if (pageStack.currentPage == tasksPage)
                        remember.addTask(edit.text);
                    else
                        remember.addList(edit.text);
                    edit.text = "";
                    newTaskDialog.accept();
                }
            }

        }
    }

    ToolBarLayout {
        id: commonTools
        visible: false
        ToolIcon {
            id: backButton;
            platformIconId: "toolbar-back";
            onClicked: pageStack.pop();
            visible: (pageStack.depth > 1);
        }

        ToolIcon {
            id: doneButton;
            platformIconId: "toolbar-done";
            onClicked: remember.markTasksCompleted();
            visible: remember.selectedTasksCount > 0;
        }

        ToolIcon {
            id: addButton;
            platformIconId: "toolbar-add"
            onClicked: newTaskDialog.open();
            visible: (pageStack.currentPage == tasksPage ||
                      pageStack.currentPage == mainPage);
        }

        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (myMenu.status === DialogStatus.Closed) ? myMenu.open() : myMenu.close()
        }
    }

    Menu {
        id: myMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Log out");
                onClicked: remember.forgetAuthToken();
            }
            MenuItem {
                text: qsTr("About");
                onClicked: aboutDialog.open();
            }
        }
    }

    QueryDialog {
        id: aboutDialog;
        titleText: qsTr("Remember - A simple application to access Remember The Milt tasks.");
        message: qsTr("<b>Remember</b> uses the Remember The Milk API but is not "
                      + "endorsed or certified by Remember The Milk.");
        acceptButtonText: qsTr("OK");
    }

    Component.onCompleted: {
        if (remember.showSplashScreen())
        {
            aboutDialog.open();
        }
    }
}
