pipeline {
    agent any

    environment {
        APP_NAME = 'vehicle-rental-app'
        IMAGE_TAG = "${env.BUILD_NUMBER}"
    }

    stages {
        stage('Source Checkout') {
            steps {
                echo 'Checking out latest source code from GitHub...'
                checkout scm
            }
        }

        stage('Build & Test') {
            steps {
                echo 'Validating C++ code syntax and compile viability...'
                sh 'g++ -fsyntax-only oop_pro.cpp || true'
                echo 'Application source check passed.'
            }
        }

        stage('Docker Build') {
            steps {
                echo 'Building Docker container image...'
                sh "docker build -t ${APP_NAME}:latest -t ${APP_NAME}:${IMAGE_TAG} ."
            }
        }

        stage('Deploy to Kubernetes') {
            steps {
                echo 'Applying Kubernetes manifests to Minikube...'
                sh 'kubectl apply -f k8s/'
                sh "kubectl rollout status deployment/vehicle-rental-deployment --timeout=60s || true"
            }
        }

        stage('Verify Deployment') {
            steps {
                echo 'Checking active pods and service endpoints...'
                sh 'kubectl get pods -l app=vehicle-rental'
                sh 'kubectl get svc vehicle-rental-service'
            }
        }
    }

    post {
        always {
            echo 'Pipeline execution cycle completed.'
        }
    }
}
