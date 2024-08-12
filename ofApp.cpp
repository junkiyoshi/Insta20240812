#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofNoFill();
}

//--------------------------------------------------------------
void ofApp::update() {

	ofColor color;

	for (int radius = 50; radius < 600; radius += 50) {

		for (int i = 0; i < 2; i++) {

			for (int k = 0; k < 2; k++) {

				auto deg = (ofGetFrameNum() * 2 + 45 * i + radius) % 360;
				auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));

				color.setHsb(ofMap((radius + ofGetFrameNum() * 3) % 255, 0, 255, 0, 255), 255, 255);

				vector<glm::vec2> log;
				log.push_back(location);
				this->log_list.push_back(log);
				this->color_list.push_back(color);
				this->life_list.push_back(ofRandom(100, 180));
				this->radius_list.push_back(radius);
			}
		}
	}

	int step = 4;
	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 2;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);
			this->radius_list.erase(this->radius_list.begin() + i);

			continue;
		}

		int count = 0;
		while (true) {

			auto deg = ofMap(ofNoise(glm::vec3(this->log_list[i].back() * 0.008, count * 0.01 + ofGetFrameNum() * 0.01)), 0, 1, -360, 360);
			auto tmp = this->log_list[i].back() + glm::vec2(step * cos(deg * DEG_TO_RAD), step * sin(deg * DEG_TO_RAD));
			if (glm::length(tmp) > this->radius_list[i] - 15 && glm::length(tmp) < this->radius_list[i] + 15) {

				this->log_list[i].push_back(this->log_list[i].back() + glm::vec2(step * cos(deg * DEG_TO_RAD), step * sin(deg * DEG_TO_RAD)));
				break;
			}
			else {

				count++;
			}
		}
		/*
		while (this->log_list[i].size() > 300) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
		*/
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofRotateZ(90);

	for (int i = 0; i < this->log_list.size(); i++) {

		if (this->life_list[i] > 30) {

			ofSetColor(this->color_list[i]);
			ofSetLineWidth(1);
		}
		else {

			ofSetColor(ofColor(this->color_list[i], ofMap(this->life_list[i], 0, 30, 0, 255)));
			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	/*
	int start = 80;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}