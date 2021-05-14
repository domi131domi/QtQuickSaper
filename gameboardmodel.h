#ifndef GAMEBOARDMODEL_H
#define GAMEBOARDMODEL_H

#include <QAbstractTableModel>
#include <QRandomGenerator>
#include <QTimer>

struct Field
{
    bool isRevieled = false;
    bool isFlagged = false;
    int nextToBombs;
    bool isBomb = false;
};

class GameBoardModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GameBoardModel(QObject *parent = nullptr);

    enum
    {
        IsBombRole = Qt::UserRole,
        NextToBombsRole,
        IsRevieledRole,
        IsFlaggedRole,
        GetWidthRole,
        GetHeightRole,
        mouseButtonRole
    };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    virtual QHash<int, QByteArray> roleNames() const override;

signals:
    void gameOver();
    void newGame();
    void winGame();
    void sizeChanged();
    void pointsChanged(int points);
public slots:
    void startNewGame();
    void startEasy();
    void startMedium();
    void startHard();
    void timeMinusPoints();
private:
    static const int MAX_HEIGHT = 20;
    static const int MAX_WIDTH = 20;
    static const int EASY_W = 10;
    static const int MEDIUM_W = 15;
    static const int HARD_W = 20;
    static const int EASY_B = 4;
    static const int MEDIUM_B = 30;
    static const int HARD_B = 60;
    static const int ADD_POINTS = 10;
    static const int MINUS_POINTS = 3;
    static const int TIMER = 2000;
    Field fields[MAX_HEIGHT][MAX_WIDTH];
    int width = 10;
    int height = 10;
    int bombs_count;
    int revieled_count;
    void prepareGame(int h, int w, int bombs);
    void show_empty(int y, int x);
    int points;
    int multi;
    bool firstMove;
    QTimer* timer;
    bool gameRunning;
};

#endif // GAMEBOARDMODEL_H
