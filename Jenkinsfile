pipeline {
  tools {
    "org.jenkinsci.plugins.docker.commons.tools.DockerTool" "docker"
  }
  agent {
    docker {
      image "mesosphere/aws-cli"
    }
  } 

  environment {
    AWS_ACCESS_KEY_ID         = credentials("jenkins-aws-secret-key-id")
    AWS_SECRET_ACCESS_KEY     = credentials("jenkins-aws-secret-access-key")
    PRODUCTION_S3_BUCKETNAME  = "telephone-rose-web-app-production"
  }

  stages {
    stage("Build") {
      steps {
        sh "npm install"
        sh "npm run build"
      }
    }

    stage("Deploy production") { 
      when { branch "master" }
      steps {
        sh "aws s3 sync build s3://${PRODUCTION_S3_BUCKETNAME} --delete --cache-control max-age=31536000,public"
        sh "aws s3 cp s3://${PRODUCTION_S3_BUCKETNAME}/index.html s3://${PRODUCTION_S3_BUCKETNAME}/index.html --metadata-directive REPLACE --cache-control max-age=0,no-cache,no-store,must-revalidate --content-type text/html --acl public-read"
      }
    }
  }
}
