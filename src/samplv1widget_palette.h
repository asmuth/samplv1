// samplv1widget_palette.h
//
/****************************************************************************
   Copyright (C) 2012-2020, rncbc aka Rui Nuno Capela. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc.,
   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*****************************************************************************/

#ifndef __samplv1widget_palette_h
#define __samplv1widget_palette_h

#include <QDialog>
#include <QPushButton>
#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QSettings>
#include <QMap>


// Forward decls.
class QListView;
class QLabel;
class QToolButton;


//-------------------------------------------------------------------------
// samplv1widget_palette

namespace Ui { class samplv1widget_palette; }


class samplv1widget_palette: public QDialog
{
	Q_OBJECT

public:

	samplv1widget_palette(QWidget *parent = nullptr,
		const QPalette& pal = QPalette());

	virtual ~samplv1widget_palette();

	void setPalette(const QPalette& pal);
	const QPalette& palette() const;

	void setSettings(QSettings *settings, bool owner = false);
	QSettings *settings() const;

	void setPaletteName(const QString& name);
	QString paletteName() const;

	bool isDirty() const;

	static QStringList namedPaletteList(QSettings *settings);
	static bool namedPalette(QSettings *settings,
		const QString& name, QPalette& pal, bool fixup = false);

	static QPalette::ColorRole colorRole(const QString& name);

	class PaletteModel;
	class ColorDelegate;
	class ColorButton;
	class ColorEditor;
	class RoleEditor;

protected slots:

	void nameComboActivated(const QString& name);
	void nameComboChanged(const QString& name);
	void saveButtonClicked();
	void deleteButtonClicked();

	void generateButtonChanged();
	void resetButtonClicked();
	void detailsCheckClicked();
	void importButtonClicked();
	void exportButtonClicked();

	void paletteChanged(const QPalette& pal);

	void accept();
	void reject();

protected:

	void setPalette(const QPalette& pal, const QPalette& parentPal);

	bool namedPalette(const QString& name, QPalette& pal);
	void saveNamedPalette(const QString& name, const QPalette& pal);
	void deleteNamedPalette(const QString& name);
	QStringList namedPaletteList();

	void updateNamedPaletteList();
	void updateGenerateButton();
	void updateDialogButtons();

	void setDefaultDir(const QString& dir);
	QString defaultDir() const;

	void setShowDetails(bool on);
	bool isShowDetails() const;

	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);

private:

	Ui::samplv1widget_palette *p_ui;
	Ui::samplv1widget_palette& m_ui;

	QSettings *m_settings;
	bool m_owner;

	QPalette m_palette;
	QPalette m_parentPalette;
	PaletteModel *m_paletteModel;
	bool m_modelUpdated;
	bool m_paletteUpdated;
	int m_dirtyCount;
	int m_dirtyTotal;
};


//-------------------------------------------------------------------------
// samplv1widget_palette::PaletteModel

class samplv1widget_palette::PaletteModel : public QAbstractTableModel
{
	Q_OBJECT
	Q_PROPERTY(QPalette::ColorRole colorRole READ colorRole)

public:

	PaletteModel(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation,
		int role = Qt::DisplayRole) const;

	void setPalette(const QPalette &palette, const QPalette &parentPalette);
	const QPalette& palette() const;

	void setGenerate(bool on) { m_generate = on; }

	QPalette::ColorRole colorRole() const { return QPalette::NoRole; }

signals:

	void paletteChanged(const QPalette &palette);

protected:

	QPalette::ColorGroup columnToGroup(int index) const;
	int groupToColumn(QPalette::ColorGroup group) const;

private:

	QPalette m_palette;
	QPalette m_parentPalette;
	QMap<QPalette::ColorRole, QString> m_roleNames;
	int m_nrows;
	bool m_generate;
};


//-------------------------------------------------------------------------
// samplv1widget_palette::ColorDelegate

class samplv1widget_palette::ColorDelegate : public QItemDelegate
{
public:

	ColorDelegate(QObject *parent = nullptr)
		: QItemDelegate(parent) {}

	QWidget *createEditor(QWidget *parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

	void setEditorData(QWidget *editor,
		const QModelIndex& index) const;
	void setModelData(QWidget *editor,
		QAbstractItemModel *model,
		const QModelIndex& index) const;

	void updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem& option,
		const QModelIndex &index) const;

	virtual void paint(QPainter *painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const;

	virtual QSize sizeHint(const QStyleOptionViewItem& option,
		const QModelIndex& index) const;
};


//-------------------------------------------------------------------------
// samplv1widget_palette::ColorButton

class samplv1widget_palette::ColorButton : public QPushButton
{
	Q_OBJECT
	Q_PROPERTY(QBrush brush READ brush WRITE setBrush)

public:

	ColorButton (QWidget *parent = nullptr);

	const QBrush& brush() const;
	void setBrush(const QBrush& b);

signals:

	void changed();

protected slots:

	void chooseColor();

protected:

	void paintEvent(QPaintEvent *event);

private:

	QBrush m_brush;
};


//-------------------------------------------------------------------------
// PaleteEditor::ColorEditor

class samplv1widget_palette::ColorEditor : public QWidget
{
	Q_OBJECT

public:

	ColorEditor(QWidget *parent = nullptr);

	void setColor(const QColor &color);
	QColor color() const;
	bool changed() const;

signals:

	void changed(QWidget *widget);

protected slots:

	void colorChanged();

private:

	samplv1widget_palette::ColorButton *m_button;
	bool  m_changed;
};


//-------------------------------------------------------------------------
// PaleteEditor::RoleEditor

class samplv1widget_palette::RoleEditor : public QWidget
{
	Q_OBJECT

public:

	RoleEditor(QWidget *parent = nullptr);

	void setLabel(const QString &label);
	void setEdited(bool on);
	bool edited() const;

signals:

	void changed(QWidget *widget);

protected slots:

	void resetProperty();

private:

	QLabel *m_label;
	QToolButton *m_button;
	bool m_edited;
};


#endif // __samplv1widget_palette_h

// end of samplv1widget_palette.h
