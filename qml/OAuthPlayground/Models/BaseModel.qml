import QtQuick 2.0

Item {
    id: baseModel
    property string source:"1"
    property string json:""
    property int status: XMLHttpRequest.UNSENT
    property ListModel model: ListModel{ id: model }
    property alias count: model.count
    signal updated()

    onSourceChanged: {
//        console.log(baseModel.source)
        if (source  === "1" || source  === ""){
            console.log("waiting on source to change")
        }else{
            console.log("Is this correct api ?  \n" + baseModel.source )

        var xhr = new XMLHttpRequest;
        xhr.open("GET", baseModel.source);

        xhr.onreadystatechange = function() {
                status = xhr.readyState;
                if (xhr.readyState == 4 && xhr.status == 200)
                    json = xhr.responseText;
            }
            xhr.send();
        }
    }
    onJsonChanged: {
        if ( json != "" ) {
            updateJSONModel()
            updated();
        }
    }
}
