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
        target: service;
        onAuthenticationDone: {
            if (!success)
            {
                loginDialog.open();
            }
        }

        onAuthenticationLoadUrl: {
            loginDialog.open();
            loginDialog.url = url;
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

    ToolBarLayout {
        id: commonTools
        visible: false
        ToolIcon {
            id: backButton;
            iconId: "icon-m-toolbar-back";
            onClicked: pageStack.pop();
            visible: (pageStack.depth > 1);
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
                onClicked: myApp.forgetAuthToken();
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
