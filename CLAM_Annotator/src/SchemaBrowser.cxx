#include "SchemaBrowser.hxx"

#include "Schema.hxx"

#include <qvariant.h>
#include <qsplitter.h>
#include <q3header.h>
#include <q3listview.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qtextbrowser.h>
#include <qlayout.h>
//#include <QtGui/QToolTip>
#include <Qt3Support/q3whatsthis.h>
#include <QtGui/qimage.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qlineedit.h>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QFrame>

#include <q3urloperator.h>

// Generated from /usr/share/icons/noia_kde_100/16x16/apps/xkill.png
static const unsigned char scopeIcon_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x1f, 0xf3, 0xff, 0x61, 0x00, 0x00, 0x01,
    0x8e, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xa5, 0xd3, 0xbd, 0x6b, 0x93,
    0x61, 0x14, 0x05, 0xf0, 0x9f, 0xf9, 0x50, 0x04, 0x21, 0x26, 0x2d, 0x12,
    0x35, 0x88, 0x15, 0xa4, 0x55, 0x0b, 0xd2, 0x52, 0x32, 0x44, 0x94, 0x2e,
    0x4a, 0x87, 0x76, 0x70, 0x50, 0x07, 0xab, 0xe0, 0x52, 0x17, 0x57, 0x11,
    0xa1, 0xd8, 0xc5, 0x5d, 0x41, 0x50, 0x57, 0xff, 0x02, 0x17, 0xf7, 0x0a,
    0x4a, 0x0b, 0x0a, 0x62, 0x44, 0x30, 0x92, 0x88, 0x8a, 0x9a, 0x6a, 0xb5,
    0x86, 0x57, 0x63, 0x9b, 0x96, 0xb7, 0xd1, 0x21, 0x85, 0x16, 0xe9, 0x9b,
    0x0e, 0x3d, 0xe3, 0x7d, 0xce, 0x3d, 0xcf, 0xe5, 0x9c, 0x7b, 0x69, 0x8f,
    0x5b, 0x78, 0xdd, 0x8e, 0x10, 0xdb, 0x40, 0xa0, 0x81, 0xd9, 0xcd, 0x08,
    0x7c, 0xc5, 0x5b, 0x64, 0xa3, 0x08, 0x5b, 0x22, 0xea, 0x97, 0xd0, 0x9f,
    0xa1, 0x84, 0xe0, 0x27, 0x47, 0x51, 0xc7, 0x7d, 0x7c, 0x5b, 0x4b, 0x8c,
    0xaf, 0xd3, 0x7c, 0x7b, 0x80, 0xb9, 0x6b, 0x74, 0x0e, 0x73, 0x36, 0xcf,
    0x50, 0x17, 0x0b, 0x35, 0x9e, 0xd7, 0x38, 0x83, 0xf7, 0x98, 0x8b, 0x9a,
    0xe0, 0xf2, 0x09, 0x1a, 0x37, 0xb8, 0xba, 0x95, 0x23, 0xcb, 0x2b, 0xdf,
    0x7e, 0xc7, 0x0b, 0x3c, 0x61, 0xac, 0xc8, 0x01, 0x5c, 0x5f, 0x3b, 0xc1,
    0x5e, 0xec, 0xc6, 0x3c, 0x06, 0xc6, 0x29, 0xf4, 0x74, 0x74, 0x9c, 0xea,
    0x9f, 0x98, 0xb0, 0x54, 0x2e, 0x5b, 0x08, 0x02, 0xcd, 0x95, 0xc7, 0x18,
    0xbd, 0x15, 0x26, 0x43, 0xfe, 0x20, 0x81, 0x4c, 0xac, 0x25, 0xec, 0x25,
    0xee, 0xee, 0xe1, 0x77, 0x8e, 0x63, 0xa9, 0xee, 0x6e, 0xf1, 0x91, 0x11,
    0xe9, 0x5c, 0xce, 0xbe, 0xbe, 0x3e, 0x3b, 0xb2, 0x59, 0x09, 0x74, 0xd2,
    0xb5, 0x8b, 0x25, 0x8c, 0x61, 0x1a, 0x4f, 0x63, 0xb8, 0x80, 0xd3, 0xb8,
    0xd3, 0x24, 0x13, 0x92, 0xa8, 0x4e, 0x4d, 0x79, 0x55, 0x28, 0x58, 0x4e,
    0x26, 0x1d, 0x1c, 0x1d, 0xb5, 0xbd, 0xb7, 0x57, 0xb8, 0x6a, 0x58, 0x06,
    0x8f, 0x71, 0x0e, 0x17, 0xe3, 0xf8, 0x84, 0x77, 0x98, 0xa9, 0x33, 0x98,
    0xc7, 0x4e, 0x0e, 0xff, 0x5a, 0x5c, 0x34, 0xdf, 0x6c, 0xaa, 0xd6, 0xeb,
    0xde, 0x94, 0x4a, 0x66, 0x83, 0x40, 0x8d, 0xcf, 0xd3, 0x14, 0x43, 0x1e,
    0x6a, 0x25, 0xf4, 0xe1, 0xff, 0x14, 0x72, 0x5f, 0x08, 0x7b, 0xd8, 0xff,
    0x97, 0xf4, 0x4c, 0x10, 0x28, 0x55, 0x2a, 0xaa, 0x41, 0xa0, 0xd1, 0x32,
    0xf2, 0xde, 0x47, 0x42, 0x3c, 0x8a, 0x4a, 0x01, 0xc6, 0xb3, 0xa4, 0x8e,
    0x73, 0x28, 0x45, 0x3e, 0x64, 0x5b, 0x9d, 0x72, 0x91, 0xc9, 0x32, 0x49,
    0xdc, 0xc4, 0x8f, 0x76, 0x02, 0x30, 0x88, 0xe1, 0x96, 0xf1, 0x1a, 0x48,
    0xe3, 0x19, 0x1e, 0xa0, 0x19, 0xd1, 0xb3, 0x2e, 0x4e, 0xe2, 0x4a, 0x3b,
    0xc2, 0x46, 0xb7, 0x30, 0x84, 0xf3, 0x9b, 0x11, 0x08, 0xb4, 0x96, 0x31,
    0x12, 0xff, 0x00, 0xa0, 0x70, 0x79, 0xe5, 0x4b, 0x59, 0xeb, 0x98, 0x00,
    0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82
};

// Generated from /usr/share/apps/kexi/icons/crystalsvg/22x22/actions/label.png
static const unsigned char attributeIcon_data[] = { 
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x16,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xc4, 0xb4, 0x6c, 0x3b, 0x00, 0x00, 0x02,
    0xd8, 0x49, 0x44, 0x41, 0x54, 0x38, 0x8d, 0xb5, 0x94, 0x4d, 0x68, 0x13,
    0x41, 0x18, 0x86, 0x9f, 0xcc, 0x56, 0xb1, 0x10, 0xc5, 0x56, 0xa5, 0x92,
    0xa6, 0x69, 0x8d, 0xe6, 0x20, 0x8a, 0xc4, 0x56, 0xb1, 0xa8, 0xf5, 0x07,
    0x22, 0x78, 0x88, 0xc5, 0x83, 0x7a, 0xf0, 0x52, 0x2c, 0xa4, 0x7a, 0x30,
    0x07, 0xa1, 0xe9, 0x41, 0x50, 0xe9, 0xa5, 0x87, 0xa6, 0xa0, 0xe0, 0xa1,
    0xb5, 0x42, 0x41, 0x0f, 0x3d, 0x78, 0x10, 0xac, 0x1e, 0x14, 0x95, 0xaa,
    0xa8, 0x28, 0xd6, 0x22, 0x56, 0x91, 0x92, 0xa2, 0xdd, 0x58, 0x52, 0x35,
    0xa5, 0xdb, 0xa6, 0x6b, 0x13, 0x28, 0xd9, 0xf1, 0x60, 0x36, 0x24, 0x69,
    0x7e, 0xbc, 0xf8, 0xc1, 0x30, 0xdf, 0xce, 0xce, 0x3c, 0xfb, 0xee, 0xfb,
    0xcd, 0x8c, 0x45, 0x4a, 0xc9, 0xff, 0x88, 0x32, 0x33, 0xb1, 0x58, 0x2c,
    0xb4, 0xb4, 0xb4, 0x20, 0x84, 0x40, 0x51, 0x94, 0xac, 0x49, 0x4b, 0x4b,
    0x4b, 0xa8, 0xaa, 0x8a, 0xaa, 0xaa, 0x4c, 0x4f, 0x4f, 0x93, 0x48, 0x24,
    0x28, 0x25, 0xa8, 0x2c, 0xf3, 0x21, 0x12, 0x89, 0xf0, 0xb9, 0xc6, 0xbf,
    0x0d, 0x83, 0x5e, 0xa0, 0x29, 0x35, 0x9c, 0x40, 0xe1, 0x07, 0x4e, 0x46,
    0x71, 0x72, 0x8f, 0xc1, 0x53, 0xb7, 0x4d, 0x21, 0xc5, 0xe0, 0x96, 0xdc,
    0x01, 0x5b, 0xeb, 0xd0, 0x8b, 0x9d, 0xf6, 0xea, 0xa6, 0xcd, 0xeb, 0xd6,
    0x01, 0x90, 0x94, 0x12, 0x43, 0x4a, 0xa2, 0xba, 0x4e, 0x58, 0xd3, 0x98,
    0x9a, 0x9b, 0x07, 0x38, 0x1b, 0xbb, 0x73, 0xba, 0x5f, 0xd7, 0xf5, 0x82,
    0xf0, 0x2c, 0xb0, 0xd7, 0xe3, 0xbb, 0x9e, 0xb4, 0x28, 0xe7, 0xcb, 0x84,
    0xc2, 0xfc, 0x6a, 0x07, 0x33, 0x55, 0xf5, 0x1c, 0xdc, 0xb2, 0x19, 0x43,
    0x4a, 0x92, 0x86, 0x91, 0xea, 0x25, 0x23, 0xe1, 0x30, 0x5a, 0x3c, 0x1e,
    0x88, 0x0c, 0x34, 0xf7, 0x00, 0x79, 0xe1, 0xc2, 0x7c, 0xe1, 0xf5, 0xf8,
    0x1e, 0x01, 0x7c, 0xb2, 0x79, 0x70, 0x78, 0xce, 0x23, 0x81, 0x8d, 0xdf,
    0x9f, 0x2f, 0x83, 0x3e, 0x1e, 0x1f, 0xc7, 0x6d, 0xaf, 0x06, 0x08, 0xda,
    0x5a, 0x87, 0xda, 0x4d, 0x5b, 0xf2, 0x82, 0x8f, 0x1d, 0x69, 0x6b, 0x07,
    0x62, 0xb3, 0x8b, 0x9f, 0xfd, 0x49, 0x65, 0x25, 0xac, 0x58, 0xc5, 0xf6,
    0x7d, 0x27, 0x71, 0x1d, 0x3a, 0xb3, 0x0c, 0xfa, 0xea, 0xd2, 0x76, 0x86,
    0x43, 0x13, 0xec, 0x77, 0x6e, 0x2a, 0x0a, 0x17, 0xa9, 0xbe, 0x1b, 0x68,
    0x59, 0x58, 0x58, 0x00, 0x98, 0x34, 0x52, 0xbe, 0xe6, 0x83, 0x0a, 0x21,
    0x78, 0xdb, 0xe9, 0xe6, 0xe5, 0xd7, 0x6f, 0x34, 0xd6, 0xd6, 0x16, 0x84,
    0x0b, 0xaf, 0xc7, 0x77, 0x1c, 0xf8, 0x0d, 0x34, 0x8c, 0x8d, 0x8d, 0x01,
    0x8c, 0x46, 0x75, 0xbd, 0x20, 0xf4, 0x6f, 0x53, 0x18, 0xed, 0xda, 0xcd,
    0x1b, 0x55, 0xa5, 0xa1, 0xc6, 0x9e, 0x86, 0x5b, 0xad, 0xd6, 0x34, 0x5c,
    0x00, 0x5e, 0xe0, 0x2e, 0xd0, 0xeb, 0xf5, 0xf8, 0x9a, 0x80, 0x7b, 0x61,
    0x4d, 0x2b, 0x0a, 0x55, 0x94, 0xbf, 0xf9, 0xc7, 0xee, 0xbd, 0xbc, 0xff,
    0x3e, 0xc5, 0x0e, 0x9b, 0x0d, 0x20, 0xb8, 0xe6, 0xd4, 0x60, 0x5a, 0xb9,
    0x00, 0x76, 0x01, 0x41, 0xe0, 0x1a, 0xd0, 0x57, 0x1f, 0xbe, 0x7f, 0x6b,
    0x6a, 0x6e, 0xbe, 0x24, 0xd4, 0xcc, 0xbf, 0x5c, 0x3d, 0xc0, 0xc7, 0x48,
    0x84, 0xad, 0x55, 0x55, 0xd9, 0xb6, 0x78, 0x3d, 0x3e, 0x0d, 0xa8, 0x7d,
    0xf0, 0xe4, 0x66, 0xcc, 0xf4, 0xc7, 0xd6, 0x3a, 0xd4, 0x56, 0x51, 0x5e,
    0x7e, 0xc3, 0x6d, 0xaf, 0x66, 0x38, 0x34, 0xc1, 0xdb, 0x4e, 0x77, 0x5e,
    0xa8, 0x99, 0x0b, 0x21, 0x70, 0xf9, 0x9f, 0xe2, 0xda, 0xb0, 0x9e, 0x50,
    0x74, 0x06, 0x20, 0x20, 0x80, 0x35, 0x80, 0x9e, 0x59, 0xd1, 0xc8, 0x40,
    0x73, 0xbf, 0x16, 0x8f, 0x07, 0x86, 0x43, 0x13, 0xda, 0x7e, 0xe7, 0x26,
    0xf6, 0x5c, 0xf9, 0x50, 0x14, 0xaa, 0x28, 0x82, 0xc9, 0xbe, 0xa3, 0x84,
    0xa2, 0x33, 0xd4, 0x55, 0x56, 0x02, 0x04, 0x05, 0x05, 0x22, 0xb5, 0xf9,
    0xbb, 0xcc, 0xea, 0xd7, 0x5f, 0x7c, 0x57, 0x10, 0x2a, 0x84, 0xc2, 0xb7,
    0x5f, 0x09, 0x00, 0x0c, 0x69, 0xa4, 0x8b, 0x17, 0x03, 0xac, 0xf9, 0x4e,
    0x4f, 0x0a, 0x1e, 0x30, 0xab, 0xbf, 0xa3, 0xe3, 0x75, 0x41, 0xe8, 0xe1,
    0xcb, 0xcf, 0x70, 0x54, 0xac, 0x25, 0xac, 0xcd, 0xa5, 0xad, 0x50, 0x01,
    0x47, 0x09, 0xe5, 0x01, 0xb3, 0xfa, 0x5b, 0x2f, 0xbc, 0x28, 0x09, 0x8d,
    0x0c, 0x34, 0xf7, 0x08, 0x60, 0x04, 0x68, 0x34, 0x41, 0x52, 0xca, 0x65,
    0x67, 0xdf, 0x84, 0x9b, 0xd5, 0x77, 0xf9, 0x9f, 0x16, 0x85, 0x9a, 0x56,
    0x3c, 0x00, 0x4e, 0x98, 0xd0, 0x7c, 0xe7, 0x3e, 0x13, 0xfe, 0xe5, 0xe7,
    0x4f, 0x5c, 0x1b, 0xd6, 0x53, 0x77, 0xee, 0x61, 0x41, 0x28, 0x80, 0x45,
    0x4a, 0xc9, 0xb1, 0x23, 0x6d, 0x06, 0x60, 0xbd, 0xff, 0xb8, 0x7f, 0x31,
    0xf7, 0x9e, 0xcd, 0xfd, 0x50, 0x6a, 0x9f, 0x06, 0xeb, 0x2a, 0x2b, 0x31,
    0xa4, 0x91, 0x17, 0x2a, 0xa5, 0x4c, 0xdf, 0x15, 0x1d, 0xc0, 0xad, 0x52,
    0xaa, 0x33, 0x95, 0x4f, 0xce, 0xce, 0x16, 0x84, 0x66, 0x79, 0xea, 0xf5,
    0xf8, 0x1e, 0x99, 0x79, 0x6e, 0xfb, 0xd7, 0xc8, 0x5c, 0x63, 0xf9, 0xd7,
    0x85, 0xc5, 0xfe, 0x22, 0x4b, 0x69, 0x2a, 0xfe, 0x00, 0x5a, 0x45, 0xc7,
    0xe3, 0x2a, 0x6a, 0x4b, 0xdb, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e,
    0x44, 0xae, 0x42, 0x60, 0x82
};


/*
 *  Constructs a SchemaBrowser as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
SchemaBrowser::SchemaBrowser( QWidget* parent, Qt::WFlags fl )
    : QWidget( parent, fl )
    , mSchema(0)
{
    scopeIcon.loadFromData( scopeIcon_data, sizeof( scopeIcon_data ), "PNG" );
    attributeIcon.loadFromData( attributeIcon_data, sizeof( attributeIcon_data ), "PNG" );
	setObjectName( "schemaBrowser" );
    schemaBrowserLayout = new QHBoxLayout( this ); 
    schemaBrowserLayout->setMargin(11);
	schemaBrowserLayout->setSpacing(6);

    splitter1 = new QSplitter( this );
    splitter1->setOrientation( Qt::Horizontal );

    attributeList = new Q3ListView( splitter1, "attributeList" );
    attributeList->addColumn( tr( "Attribute" ) );
    attributeList->addColumn( tr( "Type" ) );
    attributeList->setAllColumnsShowFocus( TRUE );
    attributeList->setRootIsDecorated( TRUE );
    attributeList->setResizeMode( Q3ListView::LastColumn );
    attributeList->setSortColumn( -1 );

    splitter2 = new QSplitter( splitter1 );
    splitter2->setOrientation( Qt::Vertical );

    attributeProperties = new QFrame( splitter2 );
    attributeProperties->setFrameShape( QFrame::StyledPanel );
    attributeProperties->setFrameShadow( QFrame::Raised );
    attributePropertiesLayout = new QGridLayout( attributeProperties ); 
	attributePropertiesLayout->setMargin(11);
	attributePropertiesLayout->setSpacing(6);

    minLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( minLabel, 0, 0 );

    minSpin = new QSpinBox( attributeProperties );
    attributePropertiesLayout->addWidget( minSpin, 0, 1 );

    maxLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( maxLabel, 1, 0 );

    maxSpin = new QSpinBox( attributeProperties );
    attributePropertiesLayout->addWidget( maxSpin, 1, 1 );

    childLabel = new QLabel( attributeProperties );
    attributePropertiesLayout->addWidget( childLabel, 2, 0 );

    childEdit = new QLineEdit( attributeProperties );
    attributePropertiesLayout->addWidget( childEdit, 2, 1 );

    attributeDocumentation = new QTextBrowser( splitter2 );
    attributeDocumentation->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)4, 0, 0, attributeDocumentation->sizePolicy().hasHeightForWidth() ) );

    schemaBrowserLayout->addWidget( splitter1 );
    languageChange();
    resize( QSize(740, 346).expandedTo(minimumSizeHint()) );
//    clearWState( WState_Polished ); // TODO: Commented out while  qt4 porting
    connect(attributeList, SIGNAL(currentChanged(Q3ListViewItem *)),
            this, SLOT(updateCurrentAttribute()));
}

/*
 *  Destroys the object and frees any allocated resources
 */
SchemaBrowser::~SchemaBrowser()
{
    // no need to delete child widgets, Qt does it all for us
}

Q3ListViewItem * lastSibling(Q3ListViewItem * item)
{
	if (!item) return item;
	Q3ListViewItem *  next = item->nextSibling();
	while (next)
	{
		item=next;
		next = item->nextSibling();
	}
	return item;
}

void SchemaBrowser::addAttribute(const std::string & scope, const std::string & name, const std::string & type)
{
    Q3ListViewItem * scopeItem = attributeList->findItem(scope.c_str(),0);
    if (!scopeItem)
    {
	    scopeItem = new Q3ListViewItem( attributeList, lastSibling(attributeList->firstChild()) );
	    scopeItem->setOpen( TRUE );
	    scopeItem->setText( 0, scope.c_str() );
	    scopeItem->setPixmap( 0, scopeIcon );
    }
    Q3ListViewItem * item = new Q3ListViewItem( scopeItem, lastSibling(scopeItem->firstChild()) );
    item->setText( 0, name.c_str() );
    item->setPixmap( 0, attributeIcon );
    item->setText( 1, type.c_str() );
}
/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void SchemaBrowser::languageChange()
{
    setWindowTitle( tr( "Schema Browser" ) );
    attributeList->header()->setLabel( 0, tr( "Attribute" ) );
    attributeList->header()->setLabel( 1, tr( "Type" ) );
    attributeList->clear();

    minLabel->setText( tr( "Minimum" ) );
    maxLabel->setText( tr( "Maximum" ) );
    childLabel->setText( tr( "Child Scope" ) );

    QString documentation = tr(
		    "<b>No item selected</b>"
		    );
    attributeDocumentation->setHtml(documentation);
}

typedef std::list<CLAM_Annotator::SchemaAttribute> SchemaAttributes;

void SchemaBrowser::setSchema(CLAM_Annotator::Schema & schema)
{
	attributeProperties->hide();
	mSchema = &schema;
	attributeList->clear();
	SchemaAttributes & attributes = schema.GetAttributes();
	for (SchemaAttributes::iterator it = attributes.begin();
			it!=attributes.end();
			it++)
	{
		addAttribute(it->GetScope(), it->GetName(), it->GetType());
	}
}

void SchemaBrowser::updateCurrentAttribute()
{
	Q3ListViewItem * current = attributeList->currentItem();
	Q3ListViewItem * parent = current->parent();
	if (!parent) // Scope
	{
		QString documentation = "<h2>Scope '" + current->text(0) + "'</h2>";
		attributeDocumentation->setHtml(documentation);
	}
	else // Attribute
	{
		const CLAM_Annotator::SchemaAttribute & attributeSchema = mSchema->GetAttribute(parent->text(0).toStdString(),current->text(0).toStdString());
		QString url = "http://mtg100.upf.es/simac/DescriptionSchemeWeb";
		Q3UrlOperator op( url );
		op.get( "index.html");

		QString documentation = "<h2>Attribute '" + parent->text(0) + "::" + current->text(0) + "'</h2>";
		documentation += "<ul>";
		documentation+="<li><b>Type:</b> ";
		documentation+=QString(attributeSchema.GetType().c_str()) + "</li>";
		if (attributeSchema.HasEnumerationValues())
		{
			documentation+="<li><b>Available Values:</b> ";
			std::list<std::string> & values = attributeSchema.GetEnumerationValues();
			const char * separator = "";
			for (std::list<std::string>::iterator it =values.begin(); it!= values.end(); it++)
			{
				documentation+=separator;
				documentation+="<tt>";
				documentation+=it->c_str();
				documentation+="</tt>";
				separator =", ";
			}
			documentation+=".</ul></li>";
		}
		if (attributeSchema.HasfRange())
		{
			documentation+="<li><b>Minimum value:</b> " + QString::number(attributeSchema.GetfRange().GetMin()) + "</li>";
			documentation+="<li><b>Maximum value:</b> " + QString::number(attributeSchema.GetfRange().GetMax()) + "</li>";
		}
		if (attributeSchema.HasiRange())
		{
			documentation+="<li><b>Minimum value:</b> " + QString::number(attributeSchema.GetiRange().GetMin()) + "</li>";
			documentation+="<li><b>Maximum value:</b> " + QString::number(attributeSchema.GetiRange().GetMax()) + "</li>";
		}
		if (attributeSchema.HasChildScope())
		{
			QString childScope = attributeSchema.GetChildScope().c_str();
			if (childScope=="") childScope = tr("No child scope");
			documentation+="<li><b>Child Scope:</b> " + childScope + "</li>";
		}
		if (attributeSchema.HasSegmentationPolicy())
		{
			documentation+="<li><b>Segmentation Policy:</b> " + QString(attributeSchema.GetSegmentationPolicy().GetString().c_str()) + "</li>";
		}
		documentation += "</ul>";
		documentation += "<div class='descriptorDocumentation'>";
		if (attributeSchema.HasDocumentation())
			documentation += attributeSchema.GetDocumentation().c_str();
		else
			documentation += "<p>The schema has no embeded documentation for the attribute.</p>";
		documentation += "</div>";
		documentation += "<p>Get further information on <a href='" + url + "'>" +url+"</a>.</p>";
		attributeDocumentation->setHtml(documentation);
	}
}


