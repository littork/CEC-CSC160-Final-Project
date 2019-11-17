void multiline(List<String> commands) {
	command = commands.join(" && ")
	return command
}

pipeline {
	agent any
	stages {
		stage('Configure CURL Build') {
			parallel {
				stage('Run CMake') {
					steps {
						echo 'Beginning Static Library CMake'
						bat "cmake -H./curl -B./curl/built -DBUILD_SHARED_LIBS=OFF"
					}
				}
			}
		}
		stage('Enable CURL SSL Support') {
			parallel {
				stage('Preprocessor Definitions') {
					steps {
						bat multiline([
							"echo \"#define USE_SSLEAY\" > setup.txt",
							"type curl/built/lib/curl_config.h >> setup.txt",
							"type setup.txt > curl/built/lib/curl_config.h"
						])
						bat multiline([
							"echo \"#define USE_OPENSSL\" > setup.txt",
							"type curl/built/lib/curl_config.h >> setup.txt",
							"type setup.txt > curl/built/lib/curl_config.h"
						])
					}
				}
			}
		}
		stage('Build CURL') {
			parallel {
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"set _CL_=/MTd",
							"Msbuild.exe ./curl/built/lib/libcurl.vcxproj /p:Configuration=Debug /p:SolutionDir=../../../"
						])
						bat "echo f | xcopy /f /y \"./curl/built/lib/Debug/libcurl-d.lib\" \"./build/libcurl/x64/Debug/libcurl.lib\""
					}
				}
				stage('Build Release x64') {
					steps {
						echo 'Beginning Release x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"set _CL_=/MT",
							"Msbuild.exe ./curl/built/lib/libcurl.vcxproj /p:Configuration=Release /p:SolutionDir=../../../"
						])
						bat "echo f | xcopy /f /y \"./curl/built/lib/Release/libcurl.lib\" \"./build/libcurl/x64/Release/libcurl.lib\""
					}
				}
			}
		}
		stage('Build Nana') {
			parallel {
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Debug /p:SolutionDir=../../../"
						])
					}
				}
				stage('Build Release x64') {
					steps {
						echo 'Beginning Release x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Release /p:SolutionDir=../../../"
						])
					}
				}
			}
		}
		stage('Build UI') {
			parallel {
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Debug /p:SolutionDir=../ /p:BuildNumber=${BUILD_NUMBER}"
						])
					}
				}
				stage('Build Release x64') {
					steps {
						echo 'Beginning Release x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Release /p:SolutionDir=../ /p:BuildNumber=${BUILD_NUMBER}"
						])
					}
				}
			}
		}
		stage('Artifact Interface') {
			parallel {
				stage('Artifact Release x64') {
					steps {
						echo 'Beginning Release x64 Artifact'
						archiveArtifacts 'build/interface/x64/Release/**/*.exe'
					}
				}
			}
		}
		stage('Generate Github Release') {
			steps {
				bat multiline([
					"call C:\\Users\\Administrator\\Desktop\\github_token.bat",
					"github-release\\github-release.exe release --user littork --repo \"CEC-CSC160-Final-Project\" --tag #${BUILD_NUMBER} --name \"Production Release\" --description \"CEC-Final-Project: Automated release for ${BRANCH_NAME} branch (Build #${BUILD_NUMBER})\""
				])
			}
		}
		stage('Upload to GitHub Release') {
			parallel {
				stage('Upload Interface x64') {
					steps {
						bat multiline([
							"call C:\\Users\\Administrator\\Desktop\\github_token.bat",
							"github-release\\github-release.exe upload --user littork --repo \"CEC-CSC160-Final-Project\" --tag #${BUILD_NUMBER} --name \"interface_x64.exe\" --file \"${JENKINS_HOME}/jobs/CEC-CSC160-Final-Project/branches/master/builds/${BUILD_NUMBER}/archive/build/interface/x64/Release/interface.exe\""
						])
					}
				}
			}
		}
	}
}
