import QtQuick 2.15
import QtQuick.Window 2.15
import GameModel 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id:window
    width: 800
    height: 800
    visible: true
    maximumHeight: 1500
    maximumWidth: 1500
    minimumHeight: 800
    minimumWidth: 800
    title: qsTr("QtQuickSaper")
    onWidthChanged: boardView.forceLayout()
    onHeightChanged: boardView.forceLayout()


    menuBar: MenuBar {
            id: menubardId
            Menu {
                title: qsTr("&Game")
                Action {
                    text: qsTr("&New Game")
                    onTriggered: gameboardmodelId.startNewGame()
                }
                MenuSeparator { }
                Action {
                    text: qsTr("&Quit")
                    onTriggered: window.close()
                }
            }
            Menu {
                title: qsTr("&Settings")
                Action {
                    text: qsTr("Easy")
                    onTriggered: gameboardmodelId.startEasy()
                }
                Action {
                    text: qsTr("Medium")
                    onTriggered: gameboardmodelId.startMedium()
                }
                Action {
                    text: qsTr("Hard")
                    onTriggered: gameboardmodelId.startHard()
                }
            }
    }

        TableView
        {
            id: boardView
            anchors.fill: parent
            columnWidthProvider: function (column) { return window.width / boardView.columns; }
            rowHeightProvider: function (row) { return (window.height - menubardId.height) / boardView.rows; }
            interactive: false;
            model: GameBoardModel {
                id: gameboardmodelId
                onNewGame: {endgametextId.newGame(); boardView.forceLayout();}
                onGameOver: endgametextId.gameLost()
                onWinGame: endgametextId.gameWon()
                onPointsChanged: pointsId.setPoints(points)
            }


            delegate: delegateItem
        }

        EndGameText {
            id: endgametextId
            anchors.centerIn: parent;

        }

        Points{
            id: pointsId
        }

        Component {
            id: delegateItem
            Rectangle
            {
                width: boardView.width / boardView.columns
                height: boardView.height / boardView.rows
                Image
                {
                    width: boardView.width / boardView.columns
                    height: boardView.height / boardView.rows
                    source: getPicture()

                    function getPicture()
                    {
                        if(!model.isRevieled)
                        {
                            if(model.isFlagged)
                                return imagesSources.flag;
                            else
                                return imagesSources.unrevieled;
                        }
                        else
                        {
                            if(model.isBomb)
                                return imagesSources.bomb;
                            var count = model.nextToBombs;
                            switch(count)
                            {
                            case 0:
                                return imagesSources.empty;
                            case 1:
                                return imagesSources.bomb1;
                            case 2:
                                return imagesSources.bomb2;
                            case 3:
                                return imagesSources.bomb3;
                            case 4:
                                return imagesSources.bomb4;
                            case 5:
                                return imagesSources.bomb5;
                            case 6:
                                return imagesSources.bomb6;
                            case 7:
                                return imagesSources.bomb7;
                            case 8:
                                return imagesSources.bomb8;
                            }
                        }
                    }

                    Item
                    {
                        id: imagesSources
                        property string bomb: "bomb.png"
                        property string empty: "empty.png"
                        property string flag: "flag.png"
                        property string bomb1: "1.png"
                        property string bomb2: "2.png"
                        property string bomb3: "3.png"
                        property string bomb4: "4.png"
                        property string bomb5: "5.png"
                        property string bomb6: "6.png"
                        property string bomb7: "7.png"
                        property string bomb8: "8.png"
                        property string unrevieled: "UnrevieledField.png"
                    }
                }

                MouseArea {
                        id: idMouseArea
                        anchors.fill: parent
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onPressed: {
                            if (idMouseArea.pressedButtons & Qt.RightButton) {
                                model.mouseButton = true;
                            } else if (idMouseArea.pressedButtons & Qt.LeftButton) {
                                model.mouseButton = false;
                            }
                        }
                }
            }
        }
}
