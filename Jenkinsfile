pipeline {
    agent any
    triggers {
        GenericTrigger(causeString: 'Relay Push Service',  token: 'Json.Build')
    }
    stages {
    
        stage('Update Submodules') {
            steps {
                sh script: 'git submodule update --init'
            }
        }

        stage('Build Project') {

            steps {
                sh script: 'cmake -DJson_BUILD_TEST=ON -DJson_AUTO_RUN_TEST=ON .'
                sh script: 'make'
            }
        }

        stage('Test Project') {
            steps {
                sh script: './Source/Json > out.ans'
                sh script: 'diff out.ans Test/inp.ans'
            }
        }

        stage('Finalize') {
            steps {
                deleteDir()
            }
        }
    }
}