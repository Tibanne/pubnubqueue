#include <QCoreApplication>
#include "Pubnub.h"

int main(int argc, char *argv[]) {
	QCoreApplication app(argc, argv);
	Pubnub s("pubsub.pubnub.com");

	return app.exec();
}

