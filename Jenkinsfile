void multiline(List<String> commands) {
	command = commands.join(" && ")
	return command
}

pipeline {
	agent any
	stages {
		stage('Build Nana') {
			parallel {
				stage('Build Debug x32') {
					steps {
						echo 'Beginning Debug x32 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Debug /p:SolutionDir=../"
						])
					}
				}
				stage('Build Debug x64') {
					steps {
						echo 'Beginning Debug x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Debug /p:SolutionDir=../"
						])
					}
				}
				stage('Build Release x32') {
					steps {
						echo 'Beginning Release x32 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Release /p:SolutionDir=../"
						])
					}
				}
				stage('Build Release x64') {
					steps {
						echo 'Beginning Release x64 Build'
						bat multiline([
							"\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Auxiliary\\Build\\vcvarsall.bat\" x86_x64",
							"Msbuild.exe ./nana/build/vc2019/nana.vcxproj /p:Configuration=Release /p:SolutionDir=../"
						])
					}
				}
			}
		}
		stage('Build UI') {
			parallel {
				stage('Build Debug x32') {
					steps {
						echo 'Beginning Debug x32 Build'
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
				stage('Build Release x32') {
					steps {
						echo 'Beginning Release x32 Build'
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
	}
	post {
		always {
			dir('Build') {
				deleteDir()
			}
		}
	}
}