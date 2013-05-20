def getCamPairStr(cam_pair)
	cam_pair_str=cam_pair.gsub('cam','camera')
	#[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F ']].each{|cam,cam_str|
		[['105',' A '],['106',' B '],['107',' C '],['108',' D '],['109',' E '],['110',' F '],['111',' A ']].each{|cam,cam_str|
		cam_pair_str=cam_pair_str.gsub(cam,cam_str)
	}
	return cam_pair_str
	
end
CapAlpLst=['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s']
#FeatIdxLst=""
#Cam_pair_cluster_condIdxLst=""
#Results_folder=""
if false

elsif true
FeatIdxLst=[0,6,4]
Cam_pair_cluster_condIdxLst=[[7,0],[31,0],[23,1],[28,1]]
Results_folder="results_20120220"
EstMethodStr="peakRight-sumOfSqW"
FeatLst=["BoVW SIFT","Color Histogram","SDALF"]
SetID="random0"
PlotEstMethodLst=["allRegion-sumOfSqW","peakRight-sumOfSqW","peakRight-sumOfSqNotToOver"]
end


