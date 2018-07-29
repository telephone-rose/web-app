pipeline {
  agent {
    dockerfile {
      filename 'Dockerfile.build'
    }
  }

  environment {
    AWS_ACCESS_KEY_ID         = credentials("jenkins-aws-secret-key-id")
    AWS_SECRET_ACCESS_KEY     = credentials("jenkins-aws-secret-access-key")
    PRODUCTION_S3_BUCKETNAME  = "app.telephone-ro.se"
  }

  stages {
    stage("Build") {
      steps {
        sh "npm ci"
        sh "npm run build"
      }
    }

    stage("Deploy production") { 
      when { branch "master" }
      steps {
        sh "aws s3 sync build s3://${PRODUCTION_S3_BUCKETNAME} --delete --acl public-read --cache-control max-age=31536000,public"
        sh "aws s3 cp s3://${PRODUCTION_S3_BUCKETNAME}/index.html s3://${PRODUCTION_S3_BUCKETNAME}/index.html --metadata-directive REPLACE --cache-control max-age=0,no-cache,no-store,must-revalidate --content-type text/html --acl public-read"
      }
    }
  }
  post {
    always {
      echo 'One way or another, I have finished'
      deleteDir() /* clean up our workspace */
    }
    success {
      echo 'I succeeeded!'
      slackSend color: 'good', message: "The pipeline ${currentBuild.fullDisplayName} completed successfully."
    }
    unstable {
      echo 'I am unstable :/'
    }
    failure {
      echo 'I failed :('
    }
    changed {
      echo 'Things were different before...'
    }
  }
}
