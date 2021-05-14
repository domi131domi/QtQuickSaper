import QtQuick 2.0

Item {
    property int points: 0

    Text {
        id: textField
        text: "Points: " + points
        font.family: "Helvetica"
        font.pointSize: 20
        color: "blue"
        anchors.left: parent.left
    }

    function setPoints(newPoints)
    {
        points = newPoints;
    }
}
