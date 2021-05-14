#include "gameboardmodel.h"

GameBoardModel::GameBoardModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    prepareGame(EASY_W, EASY_W, EASY_B);
    width = EASY_W;
    height = EASY_W;
    bombs_count = EASY_B;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this,&GameBoardModel::timeMinusPoints);
}

QVariant GameBoardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}


int GameBoardModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return height;
}

int GameBoardModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return width;
}

QVariant GameBoardModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role)
    {
        case IsBombRole:
            return QVariant(fields[index.row()][index.column()].isBomb);
        case NextToBombsRole:
            return QVariant(fields[index.row()][index.column()].nextToBombs);
        case IsRevieledRole:
            return QVariant(fields[index.row()][index.column()].isRevieled);
        case IsFlaggedRole:
            return QVariant(fields[index.row()][index.column()].isFlagged);
        case GetWidthRole:
            return QVariant(width);
        case GetHeightRole:
            return QVariant(height);
    }
    return QVariant();
}

bool GameBoardModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role != mouseButtonRole)
        return false;
    if(!gameRunning)
        return true;
    if(fields[index.row()][index.column()].isRevieled)
        return true;
    if(!value.toBool())
    {
        if(firstMove)
        {
            timer->start(TIMER);
            firstMove = false;
        }
        if(fields[index.row()][index.column()].isFlagged)
            return true;
        if(fields[index.row()][index.column()].nextToBombs == 0 && !fields[index.row()][index.column()].isBomb)
        {
            show_empty(index.row(),index.column());
            emit dataChanged(this->index(0,0), this->index(height-1,width-1));
            return true;
        }
        else if(fields[index.row()][index.column()].isBomb)
        {
            gameRunning = false;
            emit gameOver();
            timer->stop();
            fields[index.row()][index.column()].isRevieled = true;
        }
        else
        {
           revieled_count++;
           points += (ADD_POINTS + bombs_count/5);
           emit pointsChanged(points);
           fields[index.row()][index.column()].isRevieled = true;
        }
    }
    else
    {
        if(!fields[index.row()][index.column()].isRevieled)
            fields[index.row()][index.column()].isFlagged = !fields[index.row()][index.column()].isFlagged;
    }
    emit dataChanged(index, index);
    if(revieled_count == width*height - bombs_count)
    {
        gameRunning = false;
        timer->stop();
        emit winGame();
    }
    return true;
}

void GameBoardModel::show_empty(int y, int x)
{
    if(!fields[y][x].isRevieled && fields[y][x].nextToBombs == 0 && !fields[y][x].isBomb)
    {
        fields[y][x].isRevieled = true;
        revieled_count++;
    }
    else
        return;
    if(y-1 >= 0)
        show_empty(y-1,x);
    if(x-1 >= 0)
        show_empty(y,x-1);
    if(y+1 < height)
        show_empty(y+1,x);
    if(x+1 < width)
        show_empty(y,x+1);
}

QHash<int, QByteArray> GameBoardModel::roleNames() const
{
    QHash<int,QByteArray> names;
    names[IsBombRole] = "isBomb";
    names[NextToBombsRole] = "nextToBombs";
    names[IsRevieledRole] = "isRevieled";
    names[IsFlaggedRole] = "isFlagged";
    names[GetWidthRole] = "width";
    names[GetHeightRole] = "height";
    names[mouseButtonRole] = "mouseButton"; //true - left, right - false
    return names;
}



void GameBoardModel::prepareGame(int h, int w, int bombs)
{
    firstMove = true;
    gameRunning = true;
    revieled_count = 0;
    points = 0;
    for(int i=0; i < h; i++)
    {
        for(int e = 0; e < w; e++)
        {
            fields[i][e].isRevieled = false;
            fields[i][e].isBomb = false;
            fields[i][e].nextToBombs = 0;
            fields[i][e].isFlagged = false;
        }
    }
    int x,y;
    for(int i = 0; i < bombs; i++)
    {
        x = QRandomGenerator::global()->generate()%h;
        y = QRandomGenerator::global()->generate()%w;
        while(fields[x][y].isBomb)
        {
            x = QRandomGenerator::global()->generate()%h;
            y = QRandomGenerator::global()->generate()%w;
        }
        fields[x][y].isBomb = true;
        if(x+1 < h)
        {
            fields[x+1][y].nextToBombs++;
            if( y+1 < w)
                fields[x+1][y+1].nextToBombs++;
            if( y-1 >= 0)
                fields[x+1][y-1].nextToBombs++;
        }
        if(x-1 >= 0)
        {
            fields[x-1][y].nextToBombs++;
            if( y+1 < w)
                fields[x-1][y+1].nextToBombs++;
            if( y-1 >= 0)
                fields[x-1][y-1].nextToBombs++;
        }
        if( y+1 < w)
            fields[x][y+1].nextToBombs++;
        if( y-1 >= 0)
            fields[x][y-1].nextToBombs++;

    }
    emit newGame();
    emit dataChanged(index(0,0),index(height-1,width-1));
    emit pointsChanged(0);
}

void GameBoardModel::startNewGame()
{
    prepareGame(width,height,bombs_count);
}

void GameBoardModel::startEasy()
{
    prepareGame(EASY_W,EASY_W,EASY_B);
    width = EASY_W;
    height = EASY_W;
    bombs_count = EASY_B;
    emit newGame();
}

void GameBoardModel::startMedium()
{
    prepareGame(MEDIUM_W,MEDIUM_W,MEDIUM_B);
    width = MEDIUM_W;
    height = MEDIUM_W;
    bombs_count = MEDIUM_B;
    emit newGame();
}

void GameBoardModel::startHard()
{
    prepareGame(HARD_W,HARD_W,HARD_B);
    width = HARD_W;
    height = HARD_W;
    bombs_count = HARD_B;
    emit newGame();
}

void GameBoardModel::timeMinusPoints()
{
    points-= (MINUS_POINTS + bombs_count/5);
    if(points < 0)
        points = 0;
    emit pointsChanged(points);
}
