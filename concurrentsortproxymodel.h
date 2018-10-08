#ifndef CONCURRENTSORTPROXYMODEL_H
#define CONCURRENTSORTPROXYMODEL_H

#include <QAbstractProxyModel>
#include <QMap>

class ConcurrentSortProxyModel : public QAbstractProxyModel {
    Q_OBJECT
public:
    explicit ConcurrentSortProxyModel(QObject* parent = nullptr);
    ~ConcurrentSortProxyModel();

public:
    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;

    bool hasChildren(const QModelIndex &parent) const override;

    //QVariant data(const QModelIndex &index, int role) const override;
    //QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // QAbstractProxyModel interface
    void setSourceModel(QAbstractItemModel* model) override;

    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;

private:

    void sourceModelAboutToReset();
    void sourceModelReset();

    void resetMapping();

    QMap<QPersistentModelIndex, QPersistentModelIndex> mMapping;
};

#endif // CONCURRENTSORTPROXYMODEL_H
