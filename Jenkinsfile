pipeline {
    agent any

    environment {
        DOCKER_HUB_USER = 'navya0527'
        IMAGE_NAME      = 'devops-cicd-app'
        IMAGE_TAG       = "${BUILD_NUMBER}"
        DOCKER_CREDENTIALS_ID = 'dockerhub-credentials'
    }

    stages {
        stage('Checkout') {
            steps {
                echo 'Pulling latest code from GitHub...'
                checkout scm
            }
        }

        stage('Build and Test') {
            steps {
                echo 'Running tests...'
                sh 'pip3 install -r requirements.txt || pip install -r requirements.txt'
                sh 'pytest test_app.py'
            }
        }

        stage('Docker Build') {
            steps {
                echo 'Building Docker image...'
                sh "docker build -t ${DOCKER_HUB_USER}/${IMAGE_NAME}:${IMAGE_TAG} ."
                sh "docker tag ${DOCKER_HUB_USER}/${IMAGE_NAME}:${IMAGE_TAG} ${DOCKER_HUB_USER}/${IMAGE_NAME}:latest"
            }
        }

        stage('Push Image') {
            steps {
                echo 'Publishing image to Docker Hub...'
                withCredentials([usernamePassword(credentialsId: "${DOCKER_CREDENTIALS_ID}", usernameVariable: 'DH_USER', passwordVariable: 'DH_PASS')]) {
                    sh 'echo $DH_PASS | docker login -u $DH_USER --password-stdin'
                    sh "docker push ${DOCKER_HUB_USER}/${IMAGE_NAME}:${IMAGE_TAG}"
                    sh "docker push ${DOCKER_HUB_USER}/${IMAGE_NAME}:latest"
                }
            }
        }

        stage('Minikube Deployment') {
            steps {
                echo 'Applying Kubernetes manifests to Minikube...'
                sh 'kubectl apply -f k8s/deployment.yaml'
                sh 'kubectl apply -f k8s/service.yaml'
                sh "kubectl set image deployment/devops-app-deployment devops-app=${DOCKER_HUB_USER}/${IMAGE_NAME}:${IMAGE_TAG} --record || true"
            }
        }

        stage('Verification') {
            steps {
                echo 'Verifying Pods, Deployment, and Service status...'
                sh 'kubectl rollout status deployment/devops-app-deployment --timeout=60s'
                sh 'kubectl get pods -l app=devops-app'
                sh 'kubectl get svc devops-app-service'
            }
        }
    }

    post {
        always {
            sh 'docker logout || true'
        }
        success {
            echo "CI/CD Pipeline Build #${BUILD_NUMBER} completed successfully!"
        }
        failure {
            echo "CI/CD Pipeline Build #${BUILD_NUMBER} failed."
        }
    }
}
