import bb.cascades 1.3
import bb.system 1.0

NavigationPane {
    id: navPane

    attachedObjects: [
        SettingsPage {
            id: settingsPage
            onPeriodChanged: _geoLocation.setPeriod(period)
            onSmoothDataChanged: _geoLocation.setSmoothData(smoothData)
            onStationaryDetectionChanged: _geoLocation.setStationaryDetection(stationaryDetection)
            onSaveOnMediaCardChanged: _gpxFile.setSaveOnMediaCard(saveOnMediaCard)
        }
    ]

    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
        }
        settingsAction: SettingsActionItem {
            onTriggered: {
                navPane.push(settingsPage)
            }
        }
    }

    Page {
        id: mainPage

        onCreationCompleted: {
            _geoLocation.dataChanged.connect(onDataChangedUi)
            _geoLocation.error.connect(onError)
            _geoLocation.warning.connect(onWarning)
            _geoLocation.setPeriod(5)
            _geoLocation.startUpdates()
            Application.autoExit = true
            Application.aboutToQuit.connect(_gpxFile.close)
        }

        function onError(err) {
            statusLabel.text = err
            statusImage.filterColor = Color.create("#ffff3333")
        }

        function onWarning(warn) {
            statusLabel.text = warn
            statusImage.filterColor = Color.create("#ffdcd427")
        }

        function onDataChangedUi() {
            latLabel.text = _geoLocation.latitude.toFixed(8) + "°"
            lonLabel.text = _geoLocation.longitude.toFixed(8) + "°"
            accuracyLabel.text = "±" + _geoLocation.horizAccuracy.toFixed(2) + " " + qsTr("m")
            speedLabel.text = _geoLocation.speed.toFixed(1) + " " + qsTr("m/s")
            altLabel.text = _geoLocation.altitude.toFixed(2) + " " + qsTr("m")
            vertAccuracyLabel.text = "±" + _geoLocation.vertAccuracy.toFixed(2) + " " + qsTr("m")
            vertSpeedLabel.text = _geoLocation.vertSpeed.toFixed(1) + " " + qsTr("m/s")
            numSatLabel.text = _geoLocation.numSatellitesUsed + "/" + _geoLocation.numSatellitesTotal
        }

        function onDataChangedFile() {
            if (!isNaN(_geoLocation.latitude) &&
                !isNaN(_geoLocation.latitude))
            {
                _gpxFile.writeStartTrackPoint(_geoLocation.latitude, _geoLocation.longitude)
                if (!isNaN(_geoLocation.altitude)) _gpxFile.writeAltitude(_geoLocation.altitude)
                _gpxFile.writeTimestamp(_geoLocation.timestamp)
                if (!isNaN(_geoLocation.geoidHeight)) _gpxFile.writeGeoidHeight(_geoLocation.geoidHeight)
                if (_geoLocation.numSatellitesUsed > 0) _gpxFile.writeNumSatellitesUsed(_geoLocation.numSatellitesUsed)
                if (!isNaN(_geoLocation.hdop)) _gpxFile.writeHdop(_geoLocation.hdop)
                if (!isNaN(_geoLocation.vdop)) _gpxFile.writeVdop(_geoLocation.vdop)
                if (!isNaN(_geoLocation.pdop)) _gpxFile.writePdop(_geoLocation.pdop)
                _gpxFile.writeStartExtensions()
                if (!isNaN(_geoLocation.heading)) _gpxFile.writeTextElement("hdg", _geoLocation.heading.toFixed(1))
                if (!isNaN(_geoLocation.speed)) _gpxFile.writeTextElement("s", _geoLocation.speed.toFixed(1))
                if (!isNaN(_geoLocation.vertSpeed)) _gpxFile.writeTextElement("vs", _geoLocation.vertSpeed.toFixed(1))
                if (!isNaN(_geoLocation.horizAccuracy)) _gpxFile.writeTextElement("acc", _geoLocation.horizAccuracy.toFixed(2))
                if (!isNaN(_geoLocation.vertAccuracy)) _gpxFile.writeTextElement("vacc", _geoLocation.vertAccuracy.toFixed(2))
                _gpxFile.writeEndExtensions()
                _gpxFile.writeEndTrackPoint()
            }
        }

        attachedObjects: [
            SystemToast {
                id: saveToast
            }
        ]

        actions: [
            ActionItem {
                id: startAction
                title: qsTr("Record")
                ActionBar.placement: ActionBarPlacement.Signature
                enabled: true
                onTriggered: {
                    var fileName = Qt.formatDateTime(new Date, "yyyyMMdd_HHmmss") + ".gpx"
                    saveToast.body = fileName
                    _gpxFile.open(fileName)
                    _geoLocation.dataChanged.connect(mainPage.onDataChangedFile)
                    startAction.enabled = false
                    stopAction.enabled = true
                    Application.setClosePrompt(qsTr("Recording is active"),
                        qsTr("The application records location data to a GPX file. File will be finalized and closed."))
                }
                imageSource: "asset:///images/ic_location.png"
            },
            ActionItem {
                id: stopAction
                title: qsTr("Stop")
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: false
                onTriggered: {
                    _geoLocation.dataChanged.disconnect(mainPage.onDataChangedFile)
                    _gpxFile.close()
                    saveToast.show()
                    startAction.enabled = true
                    stopAction.enabled = false
                    Application.clearClosePrompt()
                }
                imageSource: "asset:///images/ic_stop.png"
            }
        ]

        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            topPadding: ui.du(2.0)
            leftPadding: topPadding
            rightPadding: leftPadding
            bottomPadding: topPadding
            Label {
                id: latLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: lonLabel
                text: "N/A°"
                textStyle.fontSize: FontSize.XXLarge
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: accuracyLabel
                text: "±N/A " + qsTr("m")
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: speedLabel
                text: "N/A " + qsTr("m/s")
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            Label {
                id: altLabel
                text: "N/A " + qsTr("m")
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: vertAccuracyLabel
                text: "±N/A " + qsTr("m")
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Label {
                id: vertSpeedLabel
                text: "N/A " + qsTr("m/s")
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            Label {
                id: numSatLabel
                text: "0/0"
                textStyle.fontSize: FontSize.Large
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.fontFamily: "Monospace"
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            ImageView {
                id: statusImage
                imageSource: "asset:///images/ic_map.png"
                scalingMethod: ScalingMethod.AspectFill
                horizontalAlignment: HorizontalAlignment.Center
            }
            Label {
                id: statusLabel
                horizontalAlignment: HorizontalAlignment.Fill
                textStyle.textAlign: TextAlign.Center
            }
        }
    }
}
