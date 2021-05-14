import QtQuick 2.0

Item {
    property string winText: "You Win!"
    property string loseText: "You Lost!"

    Text {
        id: textField
        text: ""
        font.family: "Helvetica"
        font.pointSize: 40
        color: "red"
        anchors.centerIn: parent;
    }

    function gameWon()
    {
        textField.text = winText;
        textField.color = "green";
    }

    function gameLost()
    {
        textField.text = loseText;
        textField.color = "red";
    }

    function newGame()
    {
        textField.text = "";
    }
}
