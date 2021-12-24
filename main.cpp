#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[]) {

    // ������ Qt5-���������� (�� ����������� ����������) ������ �������� ��������� ������
    QApplication app(argc, argv);

    // ������� ������, ������� ������������ ���� ����� ���������
    QWidget window;

    window.resize(250, 150); // �������� ������ ������� � ��������
    window.setWindowTitle("Simple example"); // ������������� ��������� ��� �������� ���� 
    window.show(); // ������� ������ �� �����

    // � ������� ������ exec() ��������� �������� ���� ����� ���������
    return app.exec();
}