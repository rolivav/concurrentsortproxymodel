#include "concurrentsortproxymodel.h"

ConcurrentSortProxyModel::ConcurrentSortProxyModel(QObject* parent)
    : QAbstractProxyModel(parent) {
}

ConcurrentSortProxyModel::~ConcurrentSortProxyModel() {
}

QModelIndex ConcurrentSortProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    QModelIndex sourceParent;
    if (parent.isValid()) {
        sourceParent = mapToSource(parent);
    }
    QMapIterator<QPersistentModelIndex, QPersistentModelIndex> it(mMapping);
    while (it.hasNext()) {
        it.next();
        if (it.value().row() == row && it.value().column() == column){
            return it.value();
        }
    }
    return QModelIndex();
}

QModelIndex ConcurrentSortProxyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

int ConcurrentSortProxyModel::rowCount(const QModelIndex &parent) const {
    if (parent.isValid()){
        return 0;
    }
    return mMapping.size();
}

int ConcurrentSortProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void ConcurrentSortProxyModel::setSourceModel(QAbstractItemModel *model) {
    connect(model, &QAbstractItemModel::modelAboutToBeReset, this, &ConcurrentSortProxyModel::sourceModelAboutToReset);
    connect(model, &QAbstractItemModel::modelReset, this, &ConcurrentSortProxyModel::sourceModelReset);
    QAbstractProxyModel::setSourceModel(model);

    resetMapping();
}

QModelIndex ConcurrentSortProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!proxyIndex.isValid()) {
        return QModelIndex();
    }
    return mMapping.key(proxyIndex);
}

QModelIndex ConcurrentSortProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid()){
        return QModelIndex();
    }
    return mMapping.value(sourceIndex);
}

void ConcurrentSortProxyModel::sourceModelAboutToReset(){
    beginResetModel();
}

void ConcurrentSortProxyModel::resetMapping()
{
    mMapping.clear();
    for (int i = 0; i < sourceModel()->rowCount(); i++){
        QPersistentModelIndex sourceIndex = sourceModel()->index(i, 0);
        if(!sourceIndex.isValid()){
           Q_ASSERT(true);
        }

        QPersistentModelIndex proxyIndex = createIndex(i, 0);
        mMapping.insert(sourceIndex, proxyIndex);
    }
}

void ConcurrentSortProxyModel::sourceModelReset() {
    // reset mapping
    resetMapping();
    endResetModel();
}


bool ConcurrentSortProxyModel::hasChildren(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return false;
    }
    return true;
}
