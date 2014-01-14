import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem
import "JSONListModel"
Page {
    id: personPage
    width: parent.width
    height: parent.height
    property string url
    title: "Volumes"
    GridView{
        anchors.topMargin: units.gu(3)
        cellWidth: parent.width/2
        cellHeight: parent.height/3
        anchors.top: parent.top
        model: jsonlistmodel.model
        width: parent.width
        height:parent.height
        delegate: Item {
            Image {
                id: bookCover
                source: model.volumeInfo.imageLinks.thumbnail
            }

            MouseArea {
                anchors.fill: bookCover
                onClicked: {
                    mainViewer.bookUrl = model.accessInfo.webReaderLink
                    print(mainViewer.bookUrl)
                    stack.push(bookView)
                    book.visible = true
                }
            }

            /*
            onClicked: {
                stack.push(bookCtrl)
            }
            */
        }

        JSONListModel {
            id: jsonlistmodel
            source: url
            query: "$.items[*]"
        }
    }


    BaseModel {
        id: personModel
        source: url
        property variant attributes: {
            'id': '',
            'title':'',
            'volumesCount':''
        }
        function updateJSONModel() {
            var ob = JSON.parse(json)
            attributes = {
                'id': ob.id,
                'title': ob.title
            };
            updated()
        }
    }
}
