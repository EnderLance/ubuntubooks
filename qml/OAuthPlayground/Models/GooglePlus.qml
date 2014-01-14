import QtQuick 2.0
import Ubuntu.Components 0.1
import Ubuntu.Components.ListItems 0.1 as ListItem
import "JSONListModel"
Page {
    id: personPage
    width: parent.width
    height: parent.height
    title: "Bookshelves"
    property string url
    ListView{
        model: jsonlistmodel.model
        width: parent.width
        height:parent.height

        delegate: ListItem.Subtitled {
            text: model.title
            subText: "Volumes: "+model.volumeCount
            onClicked: {
                mainViewer.bookshelfUrl = "https://www.googleapis.com/books/v1/mylibrary/bookshelves/"+(model.id).toString()+"/volumes?access_token="+oauth2.getaccessToken
                stack.push(bookshelf)
                volumesList.visible = true
            }
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
                'title': ob
            };
            updated()
        }
    }
}
