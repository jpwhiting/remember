import QtQuick 1.1
import com.nokia.meego 1.0
import QtWebKit 1.0
import com.jpwhiting 1.0

Dialog {
    id: loginDialog;
    property string url;
    property int wholeHeight: (screen.currentOrientation == Screen.Portrait ?
        screen.displayWidth : screen.displayHeight);
    property int wholeWidth : (screen.currentOrientation == Screen.Portrait ?
        screen.displayHeight : screen.displayWidth);
    style: DialogStyle {
        leftMargin: 25;
        rightMargin: 25;
    }

    title:
        Rectangle {
            id: titleRect;
            width: parent.width;
            height: titleLabel.height + 15;
            color: "red";
            Label {
                id: titleLabel;
                anchors.fill: parent;
                horizontalAlignment: Text.AlignHCenter;
                verticalAlignment: Text.AlignVCenter;
                platformStyle: LabelStyle {
                    textColor: "white";
                }
                text: "Login";
            }
        }

    content:
        Flickable {
            id: flickable;
            anchors.left: parent.left;
            width: parent.width
            height: loginDialog.wholeHeight - titleRect.height
                    - buttonRow.height - 100;
            contentWidth: Math.max(loginDialog.wholeWidth, webview.width);
            contentHeight: Math.max(loginDialog.wholeHeight, webview.height);
            clip: true;
            boundsBehavior: Flickable.StopAtBounds;

            WebView {
                id: webview;
                url: loginDialog.url;
                preferredWidth: parent.width;
                preferredHeight: parent.height;
                onLoadStarted: busy.running = true;
                onLoadFinished: busy.running = false;
            }
        }

        BusyIndicator {
            id: busy;
            visible: running;
            anchors.centerIn: parent;
            platformStyle: BusyIndicatorStyle {
                size: "large";
            }
        }

    buttons: ButtonRow {
        id: buttonRow;
        style: ButtonStyle { }
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
            text: qsTr("Press when done");
            onClicked: {
                service.authRequestToken();
                loginDialog.accept();
            }
        }
    }
}
