#ifndef __MAINWINDOW_HPP__
#define __MAINWINDOW_HPP__

#include <memory>

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QProcess>
#include <QMap>

class SelectorMainWindow : public QWidget
{
  Q_OBJECT

  SelectorMainWindow (const SelectorMainWindow&);
  SelectorMainWindow& operator= (const SelectorMainWindow&);

signals:
  void quit ();

public slots:
  void start    ();
  void closeAll ();

public:
  SelectorMainWindow  (QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
  ~SelectorMainWindow ();

  typedef struct process
  {
    QString   name;
    QString   path;
    QString   args;
    QString   winid;
    mutable QProcess *proc;
    int       op;

    process ()
    : proc( nullptr )
    , op( 1 )
    {
    }

    process (const struct process& from)
    {
      copy( from );
    }

    struct process& operator= (const struct process& from)
    {
      copy( from );
      return *this;
    }

    ~process()
    {
      if (proc)
        {
          proc->terminate();
          this->proc->waitForFinished();
          delete proc;
        }
    }

    private:
      void copy (const struct process& from)
      {
        if (this == &from)
          {
            return;
          }

        this->path  = from.path;
        this->args  = from.args;
        this->name  = from.name;
        this->winid = from.winid;
        this->op    = from.op;
                
        if (this->proc != nullptr)
          {
            this->proc->terminate();
            this->proc->waitForFinished();
            delete this->proc;
          }

        this->proc = from.proc;
        from.proc = nullptr;
      }

  } process_t;


  typedef QMap< QPushButton*, process_t > processes_t;

  std::unique_ptr< QGridLayout > m_grid; 
  QPushButton                    m_exitbutton;
  processes_t                    m_processes;
  QString                        m_findwinid;
  QString                        m_showwinid;
  QString                        m_hidewinid;

  void _findwinid( process_t& proc );
  void _showwinid( process_t& proc );
  void _hidewinid( process_t& proc );
};

#endif

