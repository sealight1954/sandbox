#!/usr/bin/ruby
# require File.dirname(__FILE__) + "/../Mysql/MysqlClass.rb"
require File.dirname(__FILE__) + "/Utils.rb"
# require File.dirname(__FILE__) + "/../lib/paramMakeCorrespondenceTable.rb"
# require File.dirname(__FILE__) + "/paramGenThesis.rb"
# @brief 各画像・各特徴抽出アルゴリズム・各パラメータをループしてtex表示用の表を作成する。
# @note texファイルでいくつか宣言しておくマクロあり（日本語テキスト置き換え用)
# @note パラメータの取得とか、正規表現ですっきり書きたいところもある。



STDOUT.sync = true
res_str=""

# 画像ごとのループ
ImgLst=["kaba" , "lena" ,  "nature"]
# ImgDir="C:/Users/ZQ83219/Documents/Visual\ Studio\ 2008/Projects/SandBox/FeatureExtractionTest/FeatureDetectionTest/output"
ImgDir="output"
# imgName = "kaba"
# imgIdx=0

FeatureLst=["SIFT" , "SURF" , "STAR" , "ORB" , "FAST" , "MSER"]
# featureParamLstLst の要素の表記は、各特徴の抽出結果についてまとめたファイルの表記と一致させること

FeatureParamLstLst=[
	["octave" , "edgeThreshold" , "contrastThreshold" , "sigma"],
	["hessianThreshold" , "octave" , "octaveLayers"],
	["maxSize" , "responseThreshold" , "lineThresholdProjected" , "lineThresholdBinarized" , "supressNonMaxSize"],
	["scaleFactor" , "nlevels" , "edgeThreshold" , "firstLevel" , "WTAK" , "scoreType", "patchSize"],
	["threshold" , "nonmaxSuppression"],
	["delta" , "minArea" , "maxArea" , "maxVariation" , "minDiversity" , "maxEvolution" , "areaThreshold" , "minMargin" , "edgeBlurSize"]
]
# 特徴ごとのループ
# FeatureLst.each_with_index{|featureName, featIdx|
featIdx = 5
featureName = FeatureLst[featIdx]
	# パラメータごとのループ
	FeatureParamLstLst[featIdx].each_with_index{ |paramName, paramIdx|
		ImgLst.each_with_index{|imgName, imgIdx|
		# paramName = "edgeThreshold"
		# paramIdx = 0
			#図表はパラメータごとに1つ作る
			# #--図開始
			# res_str+=""
			#puts res_str
			#パラメータの取りうる値のリストを　ls で取得
			detectionResultDir="#{ImgDir}/#{imgName}/#{featureName}/#{paramName}"
			cmd = "ls #{detectionResultDir} | grep .eps"
			# p "#{cmd}"
			featureParamValImgNameLst=`#{cmd}`.split("\n")
			# p featureParamValImgNameLst
#--表始めを記述
str = <<"EOS"
\\begin{figure}[htpb] %--detection_results_#{featureName}_#{paramName}
\\begin{center}
EOS
			paramVal = 0 #スコープをここまで確保
			res_str += str
 			0.upto(featureParamValImgNameLst.size - 1){|paramValIdx|
				#パラメータの値ごとのループ(1セルごとに進む)
 				paramValImgName = featureParamValImgNameLst[paramValIdx]

				# 列開始　paramValを調べる-------------------------------------------------------
				paramValLstLstFilepath = "#{detectionResultDir}/TimeLog_#{imgName}_#{featureName}_#{paramName}.txt"
				paramValLstLst=paramValLstLstFilepath.getArray
				# threshold-010_nonmaxSupression-0 58249points 4msec のフォーマットを分解
				isParamValSet = false
				paramNumOfKeypointsStr = paramValLstLst[paramValIdx].split(" ")[1]
				paramTimeStr = paramValLstLst[paramValIdx].split(" ")[2]
				paramValLstLst[paramValIdx].split(" ")[0].split("_").each{ |paramValPairStr| 
					# paramValPairStr: param-valの形のstring
					paramValPair = paramValPairStr.split("-")
					if paramValPair[0] == paramName then
						paramVal = paramValPair[1].to_f
						# p paramVal
						isParamValSet = true
						break
					end
				}
				if isParamValSet == false
					p "paramVal not found: #{paramName} in #{paramValLstLstFilepath}"
				end
				#-----------------------------------------------------------------------------
# p "paramVal:"
# p paramVal
str = <<"EOS"
\\begin{minipage}[b]{0.32\\textwidth}
\\begin{center}
%widthOfResults#{featureName} defined in .tex file
\\includegraphics[width=\\widthOfResults#{imgName}]{#{detectionResultDir}/#{paramValImgName}}\\\\
(#{paramValIdx}) #{paramName} = #{paramVal}\\\\
detect #{paramNumOfKeypointsStr} in #{paramTimeStr}
\\end{center}
\\end{minipage}
EOS
				res_str+=str
				if paramValIdx % 3 == 2 then
					res_str += "\\\\\n" # 改行挿入
				end
			}#featureParamValLst.each
#表終わり記述
str = <<"EOS"
	\\end{center}
	\\vskip-\\lastskip
	%caption#{featureName} defined in .tex file
	\\caption{#{featureName} under #{paramName} change}
	\\label{fig:results_#{imgName}_#{featureName}_#{paramName}}
	\\end{figure}
EOS
			res_str+=str
		}#ImgLst.each
		res_str+="\\clearpage\n"
	}#FeatureParamLst.each
# }#FeatureLst.each
#表の数が多すぎるため、floatメモリーを解放
puts res_str