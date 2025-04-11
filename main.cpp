#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QTimer>

#include "reactor.h" // Assuming reactorcore.h contains the ReactorCore class definition

class ReactorWindow : public QMainWindow {
    Q_OBJECT

public:
    ReactorWindow() : reactor(1.0, 300.0, 0.5, 290.0) {
        setWindowTitle("Basic Reactor Simulator");

        centralWidget = new QWidget;
        setCentralWidget(centralWidget);

        layout = new QVBoxLayout(centralWidget);

        fluxLabel = new QLabel("Neutron Flux: 0.0");
        tempLabel = new QLabel("Reactor Temperature: 0.0 K");
        powerLabel = new QLabel("Power Level: 0.0 MW");
        coolantInLabel = new QLabel("Coolant Inlet Temp: 0.0 K");
        coolantOutLabel = new QLabel("Coolant Outlet Temp: 0.0 K");

        controlRodSlider = new QSlider(Qt::Horizontal);
        controlRodSlider->setRange(0, 100); // Representing 0% to 100% insertion
        controlRodLabel = new QLabel("Control Rod Insertion: 50%");

        coolantInSpinBox = new QDoubleSpinBox();
        coolantInSpinBox->setRange(273.15, 400.0); // Example temperature range
        coolantInSpinBox->setSuffix(" K");
        coolantInLabelEdit = new QLabel("Coolant Inlet Temperature:");

        layout->addWidget(fluxLabel);
        layout->addWidget(tempLabel);
        layout->addWidget(powerLabel);
        layout->addWidget(coolantInLabel);
        layout->addWidget(coolantOutLabel);
        layout->addWidget(controlRodLabel);
        layout->addWidget(controlRodSlider);
        layout->addWidget(coolantInLabelEdit);
        layout->addWidget(coolantInSpinBox);

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &ReactorWindow::updateSimulation);
        timer->start(100); // Update every 100 milliseconds

        connect(controlRodSlider, &QSlider::valueChanged, this, &ReactorWindow::onControlRodChanged);
        connect(coolantInSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &ReactorWindow::onCoolantInletChanged);

        updateLabels();
    }

private slots:
    void updateSimulation() {
        reactor.update(0.1); // Simulate for 0.1 seconds per update
        updateLabels();
    }

    void onControlRodChanged(int value) {
        double insertion = static_cast<double>(value) / 100.0;
        reactor.setControlRodInsertion(insertion);
        controlRodLabel->setText(QString("Control Rod Insertion: %1%").arg(value));
    }

    void onCoolantInletChanged(double temp) {
        reactor.setCoolantInletTemperature(temp);
    }
    
private:
    ReactorCore reactor;
    QWidget* centralWidget;
    QVBoxLayout* layout;
    QLabel* fluxLabel;
    QLabel* tempLabel;
    QLabel* powerLabel;
    QLabel* coolantInLabel;
    QLabel* coolantOutLabel;
    QLabel* controlRodLabel;
    QSlider* controlRodSlider;
    QLabel* coolantInLabelEdit;
    QDoubleSpinBox* coolantInSpinBox;
    QTimer* timer;

    void updateLabels() {
        fluxLabel->setText(QString("Neutron Flux: %1").arg(reactor.neutronFlux, 0, 'g', 3));
        tempLabel->setText(QString("Reactor Temperature: %1 K").arg(reactor.reactorTemperature, 0, 'f', 2));
        powerLabel->setText(QString("Power Level: %1 MW").arg(reactor.powerLevel, 0, 'f', 2));
        coolantInLabel->setText(QString("Coolant Inlet Temp: %1 K").arg(reactor.coolantInletTemperature, 0, 'f', 2));
        coolantOutLabel->setText(QString("Coolant Outlet Temp: %1 K").arg(reactor.coolantOutletTemperature, 0, 'f', 2));
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    ReactorWindow window;
    window.show();
    return app.exec();
}

#include "main.moc" // Necessary for Qt's signal/slot mechanism