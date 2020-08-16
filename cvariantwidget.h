#ifndef CVARIANTWIDGET_H
#define CVARIANTWIDGET_H

/** @class class CVariantWidget
 * Класс визуального отображения варианта на вкладке этапа.
 *
 * Содержит номер исхода к которому ведёт и описывает ui варианта.
 */

#include <QWidget>

namespace Ui {
class CVariantWidget;
}

class CVariantWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CVariantWidget(QWidget *parent = nullptr);
    ~CVariantWidget();
    void setOutcomeId(qint32 outcomeId);
    QString getText();
    qint32 getOutcomeId();
    qint32 getResource();
    qint32 getResourceCount();
    void updateData(QString text, qint32 outcomeId, qint32 resource, qint32 resourceCount);

public slots:
    void slotToOutcomeClicked();

signals:
    void s_createOutcomeClicked();
    void s_toOutcomeClicked(qint32 id);

private slots:
    void on_ch_needResource_stateChanged(int arg1);

private:
    Ui::CVariantWidget *ui;
    //! id исхода к которому ведёт вариант
    qint32 m_outcomeId { -1 };
};

#endif // CVARIANTWIDGET_H
