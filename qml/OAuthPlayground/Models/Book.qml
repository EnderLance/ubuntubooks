import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.Extras.Browser 0.1

Page {
    id: personPage
    width: parent.width
    height: parent.height
    property string link
    flickable: null

    UbuntuWebView {
        id: bookview

        url: link

        anchors.fill: parent

        onLoadingChanged:
        {
            loadProgressBar.visible = loading
        }

        onLoadProgressChanged:
        {
            loadProgressBar.value = loadProgress
        }

        ProgressBar
        {
            anchors.bottom: parent.bottom
            id: loadProgressBar
            height: units.gu(1)
            width: parent.width
            minimumValue: 0
            maximumValue: 100
        }
    }
}
