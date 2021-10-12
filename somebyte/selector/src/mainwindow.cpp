#include "mainwindow.hpp"

#include <QDebug>
#include <QCoreApplication>
#include <QSettings>

#include <unistd.h>

SelectorMainWindow::SelectorMainWindow (QWidget *parent, Qt::WindowFlags flags)
: QWidget     (parent, flags)
, m_grid      (new QGridLayout( this ))
, m_findwinid ("%1/findwinid.sh %2")
, m_showwinid ("%1/showwinid.sh %2")
, m_hidewinid ("%1/hidewinid.sh %2")
{
  setWindowFlags (Qt::CustomizeWindowHint);

  QSettings settings (QCoreApplication::applicationDirPath() +
                        "/../etc/selector.conf",
                      QSettings::IniFormat);

  bool fcloseall = settings.value ("app/exit").toBool();
  bool fhide     = settings.value ("app/hexit").toBool();
  bool fontop    = settings.value ("app/ontop").toBool();

  int  scrw    = settings.value ("screen/width",  0).toInt();
  int  scrh    = settings.value ("screen/height", 0).toInt();
  bool hcenter = settings.value ("align/hcenter", 0).toBool();
  bool vcenter = settings.value ("align/vcenter", 0).toBool();
  bool bottom  = settings.value ("align/bottom",  0).toBool();
  bool right   = settings.value ("align/right",   0).toBool();
  bool vmenu   = settings.value ("align/vmenu",   0).toBool();

  int maxw = 512;
  int maxh = 512;

  int curw = settings.value ("button/width",  16).toInt();
  int curh = settings.value ("button/height", 16).toInt();

  m_exitbutton.setToolTip ("Exit");
  m_exitbutton.setMaximumSize (maxw,
                               maxh);
  m_exitbutton.setFixedSize (curw,
                             curh);
  m_exitbutton.setIcon (QIcon(":/images/exit.png"));
  m_exitbutton.setIconSize (QSize (curw-(int)(curw/4.0), curh-(int)(curh/4.0)));

  if (fontop)
    {
      setWindowFlags (Qt::ToolTip |
                      Qt::WindowStaysOnTopHint);
    }

  if (fhide)
    {
      m_exitbutton.hide();
    }

  if (!fcloseall)
    {
      QObject::connect (&m_exitbutton, SIGNAL(released()), this, SLOT(close()));
      QObject::connect (&m_exitbutton, SIGNAL(released()), (SelectorMainWindow*) this, SIGNAL(quit()));
    }
  else
    {
      QObject::connect (&m_exitbutton, SIGNAL(released()), this, SLOT(closeAll()));
    }

  int size = settings.beginReadArray ("buttons");

  for (int i = 0; i < size; ++i)
    {
      settings.setArrayIndex (i);

      QPushButton *pbutton = new (std::nothrow) QPushButton();
        
      m_processes[pbutton ].path = settings.value("path").toString(); 
      m_processes[pbutton ].args = settings.value("args").toString();
      m_processes[pbutton ].name = settings.value("name").toString();

      pbutton->setToolTip     (settings.value("text").toString()); 
      pbutton->setMaximumSize (maxw, maxh);
      pbutton->setFixedSize   (curw, curh);
      pbutton->setIcon        (QIcon (settings.value("icon").toString()));
      pbutton->setIconSize    (QSize( curw-(int)(curw/4.0), curh-(int)(curh/4.0)));

      QObject::connect (pbutton, SIGNAL(released()), (SelectorMainWindow*) this, SLOT(start()));

      if (!vmenu)
        {
          m_grid->addWidget (pbutton, 0, i, 0);
        }
      else
        {
          m_grid->addWidget (pbutton, i, 0, 0);
        }
    }

  if (!vmenu)
    {
      m_grid->addWidget (&m_exitbutton, 0, size + 1, 0);
    }
  else
    {
      m_grid->addWidget (&m_exitbutton, size + 1, 0, 0);
    }

  settings.endArray();

  adjustSize();

  int x = 0, y = 0;

  if (bottom)
    {
      y = (scrh == 0)?scrh:(scrh-height());
    }

  if (hcenter)
    {
      x = (scrw == 0)?scrw:((int)((scrw-width())/2.0)); 
    }

  if (vcenter)
    {
      y = (scrh == 0)?scrw:((int)((scrh-height())/2.0));
    }

  if (right)
    {
      x = (scrw == 0)?scrw:(scrw-width()); 
    }

  move (x, y);
}

SelectorMainWindow::~SelectorMainWindow()
{
  processes_t::iterator pos = m_processes.begin();
  while (pos != m_processes.end())
    {
      QPushButton *p = pos.key();
      pos = m_processes.erase (pos);
      delete p;
    }

}

void SelectorMainWindow::start()
{
  QPushButton *pobj = dynamic_cast<QPushButton*> (QObject::sender());

  processes_t::iterator pos = m_processes.find (pobj);
    
  if (pos != m_processes.end())
    {
      QProcess *proc = pos.value().proc;

      if (proc == nullptr)
        {
          proc = new (std::nothrow) QProcess;
          pos.value().proc = proc;
          proc->start (pos.value().path + " " + pos.value().args);
          _findwinid  (pos.value());
        }
      else
      if (proc->state() == QProcess::NotRunning)
        {
          proc->start (pos.value().path + " " + pos.value().args);
          _findwinid  (pos.value());
          qDebug()<<"stdout: "<<pos.value().winid;
        }
      else
        {
          if (! pos.value().op++)
            {
              _showwinid (pos.value());
            }
          else
            {
              pos.value().op = 0;
              _hidewinid (pos.value());
            }
        }
    }
}


void SelectorMainWindow::closeAll()
{
  processes_t::iterator pos = m_processes.begin();
  while (pos != m_processes.end())
    {
      QProcess *proc = pos.value().proc;
      pos.value().proc = nullptr;
      if (proc)
        {
          if ((proc->state() == QProcess::Running ) ||
              (proc->state() == QProcess::Starting))
            {
              proc->terminate();
              proc->waitForFinished();
            }
          delete proc;
        }
      ++pos;
    }
}

void SelectorMainWindow::_findwinid (process_t& proc)
{
    QString str = m_findwinid.arg (QCoreApplication::applicationDirPath() ).arg( proc.name);
    QProcess findproc;
    size_t i=0;

    while (i<3)
    {
      usleep (50E4);
      findproc.start (str);
      findproc.waitForFinished ();

      QByteArray array (findproc.readAllStandardOutput());

      if (array.size() && array[ array.size() - 1 ] == '\n')
        {
          array.remove( array.size()-1, 1 );
        }

      int last = array.lastIndexOf( " " );

      if (array.size())
        {
          if (last == -1)
            {
              proc.winid = array.data();
            }
          else
            {
              proc.winid = array.mid(++last).data();
            }

            break;
        }
        ++i;
    }
}

void SelectorMainWindow::_showwinid (process_t& proc)
{
  QString str = m_showwinid.arg (QCoreApplication::applicationDirPath()).arg(proc.winid);
  QProcess showproc;
  showproc.start (str);
  showproc.waitForFinished();
}

void SelectorMainWindow::_hidewinid (process_t& proc)
{   
  QString str = m_hidewinid.arg (QCoreApplication::applicationDirPath()).arg(proc.winid);
  QProcess hideproc;
  hideproc.start (str);
  hideproc.waitForFinished ();
}

