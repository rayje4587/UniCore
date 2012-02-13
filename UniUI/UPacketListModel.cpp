﻿#include "UPacketListModel.h"

#include <time.h>

#include <QMapIterator>

namespace uni
{

UPacketListModel::UPacketListModel(QList<PacketInfo> *packetInfos,QWidget *parent /*= 0*/ )
:packetInfos_(packetInfos)
{
    
}

UPacketListModel::~UPacketListModel()
{
}

QVariant UPacketListModel::data( const QModelIndex &index, int role /*= Qt::DisplayRole*/ ) const
{
    Q_ASSERT(index.isValid());
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case 0:
            {
                return tr("");
                break;
            }
        case 1:
            {
                switch(packetInfos_->at(index.row()).type)
                {
                case SendType:
                    {
                        return tr("Send");
                    }
                case RecvType:
                    {
                        return tr("Recv");
                    }
                default:
                    {
                        return tr("Unknown");
                    }
                }
                break;
            }
        case 2:
            {
                //将封包ID格式化为8位16进制。
                return tr("%1").arg(QString::number(packetInfos_->at(index.row()).id,16),8,QChar(' '));
                break;
            }
        case 3:
            {
                return packetInfos_->at(index.row()).name;
                break;
            }
        default:
            {
                return QVariant();
            }
        }
    }
    else if(role == Qt::CheckStateRole)
    {
        if(index.column() == 0)
        {
            if(packetInfos_->at(index.row()).visible)
            {
                return Qt::Checked;
            }
            else
            {
                return Qt::Unchecked;
            }
        }
        return QVariant();
    }
    return QVariant();
}

int UPacketListModel::rowCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    return packetInfos_->size();
}

int UPacketListModel::columnCount( const QModelIndex &parent /*= QModelIndex()*/ ) const
{
    return 4;
}

void UPacketListModel::addPacketID(PacketType type,int packetID)
{
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    PacketInfo packetInfo;
    packetInfo.id = packetID;
    packetInfo.type = type;
    //查看是否已经存在。
    for(int i = 0; i < packetInfos_->size(); i++)
    {
        if(packetInfos_->at(i).id == packetInfo.id
            && packetInfos_->at(i).type == packetInfo.type)
        {
            return;
        }
    }
    packetInfos_->push_back(packetInfo);
    endInsertRows();
}

QVariant UPacketListModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
    if(orientation == Qt::Horizontal
        && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            {
                return tr("Visible");
                break;
            }
        case 1:
            {
                return tr("Type");
                break;
            }
        case 2:
            {
                return tr("ID");
                break;
            }
        case 3:
            {
                return tr("Name");
                break;
            }
        default:
            {
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags UPacketListModel::flags( const QModelIndex &index ) const
{
    if(!index.isValid())
    {
        return 0;
    }

    if(index.column() == 0)
    {
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool UPacketListModel::setData( const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole*/ )
{
    if(index.column() == 0 && role == Qt::CheckStateRole)
    {
        if(value == Qt::Checked)
        {
            (*packetInfos_)[index.row()].visible = true; 
        }
        else
        {
            (*packetInfos_)[index.row()].visible = false;
        }
        emit dataChanged(index,index);
        emit visibilityChanged();
        return true;
    }
    else
    {
        return false;
    }
}

void UPacketListModel::selectAll()
{
    for(int i = 0; i < packetInfos_->size(); i++)
    {
        (*packetInfos_)[i].visible = true;
    }
    
    emit dataChanged(index(0,0),index(rowCount(),0));
    emit visibilityChanged();
}

void UPacketListModel::deselectAll()
{
    for(int i = 0; i < packetInfos_->size(); i++)
    {
        (*packetInfos_)[i].visible = false;
    }
    emit dataChanged(index(0,0),index(rowCount(),0));
    emit visibilityChanged();
}

}//namespace uni