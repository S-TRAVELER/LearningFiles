#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButon;

class FindDialog:public QDialog{
	Q_OBJECT	// 对于所有定义了信号和槽函数来说，在类定义开始处加Q_OBJECT宏是必需的。
public:
	FindDialog(QWidget* parent=0);

signals:
	void findNext(const QString &str, Qt::CaseSensitivity cs);	// Qt::CaseSensitivity 是Qt的枚举类型
	void findPrevious(const QString &str, Qt::CaseSensitivity cs);

private slots:
	void findClicked();
	void enableFindButton(const QString &text);

private:
	QLabel *label;
	QLineEdit *lineEdit;
	QCheckBox *caseCheckBox;
	QCheckBox *backwardCheckBox;
	QPushButton *findButton;
	QPushButton *closeButton;

};


#endif /*FINDDIALOG_H*/
