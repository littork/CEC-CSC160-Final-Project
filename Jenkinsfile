void multiline(List<String> commands) {
	command = commands.join(" && ")
	return command
}

pipeline {
	agent any
	stages {
		stage('Build Nana') {
			parallel {
				stage('Build Debug x86') {
					steps {
						echo 'Beginning Debug x86 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Debug /p:SolutionDir=../../../"
						])
					}
				}
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Debug /p:SolutionDir=../../../"
						])
					}
				}
				stage('Build Release x86') {
					steps {
						echo 'Beginning Release x86 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Release /p:SolutionDir=../../../"
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
				stage('Build Debug x86') {
					steps {
						echo 'Beginning Debug x86 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Debug /p:SolutionDir=../"
						])
					}
				}
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Debug /p:SolutionDir=../"
						])
					}
				}
				stage('Build Release x86') {
					steps {
						echo 'Beginning Release x86 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Release /p:SolutionDir=../"
						])
					}
				}
				stage('Build Release x64') {
					steps {
						echo 'Beginning Release x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./interface/interface.vcxproj /p:Configuration=Release /p:SolutionDir=../"
						])
					}
				}
			}
		}
		stage('Artifact Interface') {
			parallel {
				stage('Artifact Release x86') {
					steps {
						echo 'Beginning Release x86 Artifact'
						archiveArtifacts 'build/interface/x86/Release/**/*.exe'
					}
				}
				stage('Artifact Release x64') {
					steps {
						echo 'Beginning Release x64 Artifact'
						archiveArtifacts 'build/interface/x64/Release/**/*.exe'
					}
				}
			}
		}
		stage('GitHub Release') {
			steps {
				bat multiline([
					"call C:\\Users\\Administrator\\Desktop\\github_token.bat",
					"echo Upload Interface x64",
					"github-release\\github-release.exe upload --user littork --repo \"CEC-CSC160-Final-Project\" --tag \"Build #${BUILD_NUMBER}\" --name \"interface_x64.exe\" --file \"${JENKINS_HOME}/jobs/CEC-CSC160-Final-Project/branches/master/builds/${BUILD_NUMBER}/archive/build/interface/x64/Release/interface.exe\"",
					"echo Upload Interface x86",
					"github-release\\github-release.exe upload --user littork --repo \"CEC-CSC160-Final-Project\" --tag \"Build #${BUILD_NUMBER}\" --name \"interface_x86.exe\" --file \"${JENKINS_HOME}/jobs/CEC-CSC160-Final-Project/branches/master/builds/${BUILD_NUMBER}/archive/build/interface/x86/Release/interface.exe\""
				])
			}
		}
	}
}
