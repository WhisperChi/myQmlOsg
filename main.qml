import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import qmlosg.globe 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("myQmlOsg")

    GlobeOsg {
        id: globe1
        width: parent.width
        height: parent.height

        focus: true

        MouseArea {
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            onPressed: {
                globe1._mousePressEvent(mouse.x, mouse.y);
                globe1.home()
                globe1.invisiable()
            }

            onPositionChanged: {
                globe1._mouseDragEvent(mouse.x, mouse.y);
            }
        }


    }
}
