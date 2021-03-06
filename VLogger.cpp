#include "VLogger.h"

VLogger::VLogger( QWidget *parent ) : QTextEdit( parent )
{
	QFont font;
	font.setFamily( "monospace" );
	font.setFixedPitch( true );
	font.setPointSize( 9 );

	this->setFont( font );
	this->setTabStopWidth( 4 * QFontMetrics( font ).width(" ") );

	logToFile = true;
	logFile.setFileName( QCoreApplication::applicationDirPath() + "/dia.log" );

	this->setReadOnly( true );
	this->setWordWrapMode( QTextOption::NoWrap );

	coutRedirector = new StdRedirector<>( std::cout , coutCallback , this );
	cerrRedirector = new StdRedirector<>( std::cerr , cerrCallback , this );
	clogRedirector = new StdRedirector<>( std::clog , clogCallback , this );
}

VLogger::~VLogger()
{
	// Restore the streams
	delete coutRedirector;
	delete cerrRedirector;
	delete clogRedirector;
}

void VLogger::stdAppend( const QString &string , stream type )
{
	QString info( string );
	info = info.trimmed();

	if( !info.isEmpty() )
	{
		switch( type )
		{
			case cout:
				this->setTextColor( Qt::black );
				break;
			case cerr:
				this->setTextColor( Qt::red );
				break;
			case clog:
				this->setTextColor( Qt::blue );
				break;
			default: {}
		}

		this->append( QString( info ) );
		scrollToLatest();
	}
}

void VLogger::resizeEvent( QResizeEvent *event )
{
	scrollToLatest();

	QTextEdit::resizeEvent( event );
}

void VLogger::scrollToLatest( void )
{
	QScrollBar *bar = this->horizontalScrollBar();
	bar->setSliderPosition( bar->minimum() );

	bar = this->verticalScrollBar();
	bar->setSliderPosition( bar->maximum() );
}
