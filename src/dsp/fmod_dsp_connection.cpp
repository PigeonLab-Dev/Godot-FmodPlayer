#include "fmod_dsp_connection.h"
#include "fmod_dsp.h"

namespace godot {
	void FmodDSPConnection::_bind_methods() {
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_STANDARD);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_SIDECHAIN);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_SEND);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_SEND_SIDECHAIN);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_PREALLOCATED);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_MAX);
		BIND_ENUM_CONSTANT(DSPCONNECTION_TYPE_FORCEINT);

		ClassDB::bind_method(D_METHOD("set_mix", "volume_db"), &FmodDSPConnection::set_mix);
		ClassDB::bind_method(D_METHOD("get_mix"), &FmodDSPConnection::get_mix);

		ClassDB::bind_method(D_METHOD("set_mix_matrix", "matrix", "outchannels", "inchannels", "inchannel_hop"), &FmodDSPConnection::set_mix_matrix, DEFVAL(0));
		ClassDB::bind_method(D_METHOD("get_mix_matrix", "outchannels", "inchannels", "inchannel_hop"), &FmodDSPConnection::get_mix_matrix, DEFVAL(0));

		ClassDB::bind_method(D_METHOD("get_input"), &FmodDSPConnection::get_input);
		ClassDB::bind_method(D_METHOD("get_output"), &FmodDSPConnection::get_output);
		ClassDB::bind_method(D_METHOD("get_type"), &FmodDSPConnection::get_type);

		ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "mix"), "set_mix", "get_mix");
	}

	FmodDSPConnection::FmodDSPConnection() {

	}

	FmodDSPConnection::~FmodDSPConnection() {
		if (dsp_connection) {
			dsp_connection->setUserData(nullptr);
		}
		dsp_connection = nullptr;
	}

	void FmodDSPConnection::setup(FMOD::DSPConnection* p_connection) {
		ERR_FAIL_COND_MSG(!p_connection, "DSPConnection pointer is null");

		if (dsp_connection) {
			dsp_connection->setUserData(nullptr);
		}

		dsp_connection = p_connection;
		dsp_connection->setUserData(this);
	}

	void FmodDSPConnection::set_mix(const float volume_db) {
		ERR_FAIL_COND(!dsp_connection);
		FMOD_ERR_CHECK(dsp_connection->setMix((float)FmodUtils::dbfs_to_sample(volume_db)));
	}

	float FmodDSPConnection::get_mix() const {
		if (!dsp_connection) return 0.0f;
		float volume = 0.0f;
		FMOD_ERR_CHECK(dsp_connection->getMix(&volume));
		return (float)FmodUtils::sample_to_dbfs(volume);
	}

	void FmodDSPConnection::set_mix_matrix(
		const PackedFloat32Array& matrix,
		const int outchannels,
		const int inchannels,
		const int inchannel_hop
	) {
		ERR_FAIL_COND(!dsp_connection);
		ERR_FAIL_COND_MSG(matrix.size() == 0, "Matrix data is empty");
		ERR_FAIL_COND_MSG(outchannels <= 0, "outchannels must be > 0");
		ERR_FAIL_COND_MSG(inchannels <= 0, "inchannels must be > 0");

		// 验证矩阵大小是否符合预期
		int expected_size = outchannels * inchannels;
		if (inchannel_hop > 0) {
			// 如果指定了 hop，矩阵大小应该是 outchannels * inchannel_hop
			expected_size = outchannels * inchannel_hop;
		}

		ERR_FAIL_COND_MSG(matrix.size() < expected_size,
			vformat("Matrix size (%d) is smaller than expected (%d)", matrix.size(), expected_size));

		// 调用 FMOD API
		FMOD_ERR_CHECK(dsp_connection->setMixMatrix(
			const_cast<float*>(matrix.ptr()),  // FMOD 需要非 const 指针
			outchannels,
			inchannels,
			inchannel_hop
		));
	}

	PackedFloat32Array FmodDSPConnection::get_mix_matrix(
		const int outchannels,
		const int inchannels,
		const int inchannel_hop
	) const {
		ERR_FAIL_COND_V_MSG(!dsp_connection, PackedFloat32Array(), "DSPConnection is null");
		ERR_FAIL_COND_V_MSG(outchannels <= 0, PackedFloat32Array(), "outchannels must be > 0");
		ERR_FAIL_COND_V_MSG(inchannels <= 0, PackedFloat32Array(), "inchannels must be > 0");

		// 计算矩阵大小：输出通道数 × 输入通道数
		const int matrix_size = outchannels * inchannels;

		// 分配内存获取实际数据
		PackedFloat32Array matrix;
		matrix.resize(matrix_size);

		// 获取实际值（FMOD 可能会修改这些值）
		int actual_outchannels = outchannels;
		int actual_inchannels = inchannels;

		FMOD_ERR_CHECK_V(dsp_connection->getMixMatrix(
			matrix.ptrw(),								// 可写指针
			&actual_outchannels,
			&actual_inchannels,
			inchannel_hop
		), PackedFloat32Array());

		return matrix;
	}

	Ref<FmodDSP> FmodDSPConnection::get_input() const {
		ERR_FAIL_COND_V(!dsp_connection, Ref<FmodDSP>());
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_ERR_CHECK_V(dsp_connection->getInput(&dsp_ptr), Ref<FmodDSP>());
		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->setup(dsp_ptr);
		return dsp;
	}

	Ref<FmodDSP> FmodDSPConnection::get_output() const {
		ERR_FAIL_COND_V(!dsp_connection, Ref<FmodDSP>());
		FMOD::DSP* dsp_ptr = nullptr;
		FMOD_ERR_CHECK_V(dsp_connection->getOutput(&dsp_ptr), Ref<FmodDSP>());
		Ref<FmodDSP> dsp;
		dsp.instantiate();
		dsp->setup(dsp_ptr);
		return dsp;
	}

	FmodDSPConnection::FmodDSPConnectionType FmodDSPConnection::get_type() const {
		ERR_FAIL_COND_V(!dsp_connection, DSPCONNECTION_TYPE_STANDARD);
		FMOD_DSPCONNECTION_TYPE connection_type = FMOD_DSPCONNECTION_TYPE_STANDARD;
		FMOD_ERR_CHECK_V(dsp_connection->getType(&connection_type), DSPCONNECTION_TYPE_STANDARD);
		FmodDSPConnectionType to = static_cast<FmodDSPConnectionType>((int)connection_type);
		return to;
	}
}
